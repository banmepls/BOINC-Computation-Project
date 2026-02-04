# BOINC Distributed Computing Project

A complete BOINC server setup with distributed computation applications including Pi calculation, Euler's constant, Ax=b solver, and Collatz conjecture search. Features homomorphic encryption (Microsoft SEAL) for privacy-preserving CPU temperature monitoring.

## 🚀 Quick Start Guide

Follow these steps to set up and run the BOINC project. Just copy and paste each command block!

---

## Step 1: Start Docker Containers

```bash
docker compose up -d --build
```

Wait for all containers to start (db, apache, backend).

---

## Step 2: Connect to Backend Container

```bash
docker compose exec backend bash
```

You're now inside the backend container. All following commands run here.

---

## Step 3: Install Dependencies

```bash
apt update
apt install -y dos2unix nlohmann-json3-dev libzip-dev git cmake build-essential libssl-dev python3-mysqldb php-cli php-mysql lm-sensors
```

---

## Step 4: Fix Python Environment

```bash
rm -f /usr/lib/python*/EXTERNALLY-MANAGED
```

---

## Step 5: Prepare Scripts

```bash
cd /project/boinc_tutorial/scripts/
dos2unix *.sh
chmod +x *.sh
cd /project
```

---

## Step 6: Install SEAL Library

```bash
./install_seal.sh
```

This will take a few minutes. Wait for completion.

---

## Step 7: Build BOINC from Source

```bash
cd /project/boinc_source
git clone https://github.com/BOINC/boinc.git .
./_autosetup
./configure --disable-client --disable-manager
make -j4
make install
```

This takes 10-15 minutes. Be patient!

---

## Step 8: Compile Applications

```bash
cd /project/boinc_tutorial/src
make clean
make
```

You should see: `pi_compute`, `euler_compute`, `axb_compute`, `collatz_search`, `generate_keys`, `process_results`

---

## Step 9: Generate SEAL Encryption Keys

```bash
cd /project/boinc_tutorial/src
./generate_keys
```

This creates `seal_public.key` and `seal_secret.key`.

---

## Step 10: Set Up MySQL Database

```bash
mysql -u root -prootpass123 -h db -e "CREATE DATABASE IF NOT EXISTS boinc_project;"
mysql -u root -prootpass123 -h db -e "GRANT ALL PRIVILEGES ON boinc_project.* TO 'boincadm'@'%' IDENTIFIED BY 'boinc123';"
mysql -u root -prootpass123 -h db -e "FLUSH PRIVILEGES;"
```

---

## Step 11: Create BOINC Project

```bash
cd /project/boinc_source/tools

# Patch make_project for Docker environment
python3 << 'EOF'
import re
with open('make_project', 'r') as f:
    content = f.read()
content = content.replace('os.getlogin()', "'root'")
content = content.replace("os.environ['USER']", "os.environ.get('USER', 'root')")
with open('make_project', 'w') as f:
    f.write(content)
EOF

# Create the project
export USER=root
yes | ./make_project --delete_prev_inst --drop_db_first \
    --project_root /project/boinc_project \
    --url_base http://apache/boinc_project/ \
    --db_host db \
    --db_user boincadm \
    --db_passwd boinc123 \
    --project_host localhost \
    boinc_project
```

---

## Step 12: Deploy Applications

```bash
# Copy compiled applications to project
mkdir -p /project/boinc_project/apps/pi_compute/1.0/x86_64-pc-linux-gnu
mkdir -p /project/boinc_project/apps/euler_compute/1.0/x86_64-pc-linux-gnu
mkdir -p /project/boinc_project/apps/axb_compute/1.0/x86_64-pc-linux-gnu
mkdir -p /project/boinc_project/apps/collatz_search/1.0/x86_64-pc-linux-gnu

cp /project/boinc_tutorial/src/pi_compute /project/boinc_project/apps/pi_compute/1.0/x86_64-pc-linux-gnu/
cp /project/boinc_tutorial/src/euler_compute /project/boinc_project/apps/euler_compute/1.0/x86_64-pc-linux-gnu/
cp /project/boinc_tutorial/src/axb_compute /project/boinc_project/apps/axb_compute/1.0/x86_64-pc-linux-gnu/
cp /project/boinc_tutorial/src/collatz_search /project/boinc_project/apps/collatz_search/1.0/x86_64-pc-linux-gnu/

# Copy SEAL keys
cp /project/boinc_tutorial/src/seal_public.key /project/boinc_project/download/
cp /project/boinc_tutorial/src/seal_secret.key /project/boinc_project/
```

---

## Step 13: Register Applications in Database

```bash
mysql -u root -prootpass123 -h db boinc_project << 'EOF'
INSERT IGNORE INTO app (name, user_friendly_name, create_time) VALUES 
    ('pi_compute', 'PI Computation', UNIX_TIMESTAMP()),
    ('euler_compute', 'Euler Constant Computation', UNIX_TIMESTAMP()),
    ('axb_compute', 'Ax=b Solver', UNIX_TIMESTAMP()),
    ('collatz_search', 'Collatz Search', UNIX_TIMESTAMP());

INSERT IGNORE INTO platform (name, user_friendly_name, deprecated, create_time) VALUES 
    ('x86_64-pc-linux-gnu', 'Linux x86-64', 0, UNIX_TIMESTAMP());
EOF
```

---

## Step 14: Update Application Versions

```bash
cd /project/boinc_project
yes | ./bin/update_versions
```

---

## Step 15: Set Permissions

```bash
chown -R www-data:www-data /project/boinc_project
chmod -R 775 /project/boinc_project
chmod +x /project/boinc_project/cgi-bin/*
```

---

## Step 16: Configure Apache (Exit Backend Container First)

```bash
exit  # Exit the backend container
```

Now run these commands from your host machine:

```bash
# Copy Apache configuration
docker compose exec apache bash -c "cat > /etc/apache2/sites-available/boinc_project.conf << 'EOF'
    ## Settings for BOINC project boinc_project

    Alias /boinc_project/download /var/www/html/boinc_project/download
    Alias /boinc_project/stats /var/www/html/boinc_project/html/stats
    Alias /boinc_project/user_profile /var/www/html/boinc_project/html/user_profile
    Alias /boinc_project /var/www/html/boinc_project/html/user

    Alias /boinc_project_ops /var/www/html/boinc_project/html/ops
    ScriptAlias /boinc_project_cgi /var/www/html/boinc_project/cgi-bin

    <Directory \"/var/www/html/boinc_project/keys\">
        Require all denied
    </Directory>
    <Directory \"/var/www/html/boinc_project/upload\">
        Require all denied
    </Directory>

    <Directory \"/var/www/html/boinc_project/download\">
        RemoveType .php .phtml
        Require all granted
    </Directory>

    <Directory \"/var/www/html/boinc_project/html\">
        Options Indexes MultiViews
        AllowOverride AuthConfig
        Require all granted
    </Directory>

    <Directory \"/var/www/html/boinc_project/cgi-bin\">
        Options ExecCGI
        AllowOverride AuthConfig
        Require all granted
    </Directory>
EOF"

# Enable the site
docker compose exec apache a2ensite boinc_project
docker compose exec apache a2enmod cgi
docker compose exec apache apache2ctl graceful
```

---

## 🧪 Test the Applications

### Test Pi Calculation

```bash
docker compose exec backend bash -c "
    mkdir -p /tmp/pi_test && cd /tmp/pi_test
    echo '100000' > in
    cp /project/boinc_tutorial/src/seal_public.key .
    /project/boinc_tutorial/src/pi_compute
    cat out
"
```

**Expected Output:**
```
PI Computation Results
======================
Total iterations: 100000
Estimated value of PI: 3.138960000000000
Error from actual PI: 0.002632653589793

--- SEAL ENCRYPTED DATA ---
ENC_TEMP_START
<hex-encoded encrypted temperature>
ENC_TEMP_END
```

### Test Euler's Constant

```bash
docker compose exec backend bash -c "
    mkdir -p /tmp/euler_test && cd /tmp/euler_test
    echo '100000' > in
    cp /project/boinc_tutorial/src/seal_public.key .
    /project/boinc_tutorial/src/euler_compute
    cat out
"
```

**Expected Output:**
```
Total trials: 100000
Estimated e: 2.718340000000000

--- SEAL ENCRYPTED DATA ---
ENC_TEMP_START
<hex-encoded encrypted temperature>
ENC_TEMP_END
```

### Test Temperature Decryption

```bash
docker compose exec backend bash -c "
    mkdir -p /tmp/test_results
    cp /tmp/pi_test/out /tmp/test_results/pi_result_1.txt
    cp /tmp/euler_test/out /tmp/test_results/euler_result_1.txt
    /project/boinc_tutorial/src/process_results /tmp/test_results /project/boinc_tutorial/src/seal_secret.key 70.0
"
```

**Expected Output:**
```
Processed result from "euler_result_1.txt": 51.4384 C
Processed result from "pi_result_1.txt": 52.1331 C
-------------------------------------------
Number of readings: 2
Average Temperature: 51.7858 C
Threshold: 70 C
Temperature within safe range.
-------------------------------------------
```

---

## 📊 Check Results

### View Database Status

```bash
docker compose exec backend mysql -u root -prootpass123 -h db boinc_project -e "SELECT name FROM app;"
docker compose exec backend mysql -u root -prootpass123 -h db boinc_project -e "SELECT COUNT(*) FROM result WHERE outcome=1;"
```

### Access Web Interface

Open your browser and navigate to:
```
http://localhost:8080/boinc_project/
```

---

## 📚 Additional Documentation

- **[RESULTS_DEMONSTRATION.md](RESULTS_DEMONSTRATION.md)** - Complete test results with analysis
- **[HOW_TO_CHECK_RESULTS.md](HOW_TO_CHECK_RESULTS.md)** - Guide for checking computation results
- **[walkthrough.md](.gemini/antigravity/brain/.../walkthrough.md)** - Full project walkthrough

---

## 🎯 What This Project Does

1. **Pi Calculation**: Monte Carlo method with 100K iterations (99.9% accuracy)
2. **Euler's Constant**: Monte Carlo method with 100K trials (99.998% accuracy)
3. **Ax=b Solver**: Ulam-von Neumann Monte Carlo method for linear systems
4. **Collatz Search**: Distributed search for longest Collatz sequences
5. **SEAL Encryption**: Homomorphic encryption for CPU temperature monitoring

---

## 🔧 Troubleshooting

### Docker containers won't start
```bash
docker compose down
docker compose up -d --build
```

### Applications won't compile
```bash
cd /project/boinc_tutorial/src
make clean
make
```

### Database connection issues
```bash
docker compose exec backend mysql -u root -prootpass123 -h db -e "SHOW DATABASES;"
```

### Apache not serving pages
```bash
docker compose exec apache apache2ctl configtest
docker compose exec apache apache2ctl restart
```

---

## 📝 Project Structure

```
BOINC-Computation-Project/
├── boinc_tutorial/
│   ├── src/                    # Application source code
│   │   ├── pi_compute.cpp
│   │   ├── euler_compute.cpp
│   │   ├── Axb-MonteCarlo.cpp
│   │   ├── collatz_search.cpp
│   │   ├── generate_keys.cpp
│   │   ├── process_results.cpp
│   │   └── boinc_aug.h        # SEAL utilities
│   └── scripts/               # Setup scripts
├── boinc_project/             # BOINC server files
│   ├── apps/                  # Application binaries
│   ├── download/              # Client downloads
│   └── upload/                # Result uploads
└── docker-compose.yaml        # Docker configuration
```

---

## 🎓 Requirements Completed

- ✅ **Requirement 1**: Pi calculation using Monte Carlo method
- ✅ **Requirement 2**: Euler's constant calculation
- ✅ **Requirement 3**: Ax=b solver using Ulam-von Neumann method
- ✅ **Requirement 4**: Collatz conjecture search
- ✅ **Requirement 5**: SEAL homomorphic encryption for CPU temperature monitoring

---

## 👥 Authors

Created for CIP (Computation in Practice) course project.

## 📄 License

See LICENSE file for details.
