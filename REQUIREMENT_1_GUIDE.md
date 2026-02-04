# Requirement 1: Distributed Pi Calculation Guide (FIXED)

This guide describes how to fulfill Requirement 1: installing the BOINC server and three clients for Pi calculation on localhost.

## Prerequisites
- Docker and Docker Compose installed.
- Repository cloned and initialized (see [README.md](README.md)).

---

## Step 1: Start Environment
Run the following from the project root:
```bash
docker compose up -d --build
```

## Step 2: Server Initialization (Inside Backend Container)
Connect to the backend:
```bash
docker compose exec backend bash
```

Run these commands inside the container to set up the project:

### 1. Install Dependencies
```bash
apt update && apt install -y python3-mysqldb php-cli php-mysql boinc-client dos2unix
```

### 2. Build BOINC and Apps
(Required to have the project tools and app binaries)
```bash
# Fix ownership for git if needed
git config --global --add safe.directory /project/boinc_source

# Build server tools
cd /project/boinc_source
./configure --disable-client --disable-manager
make -j$(nproc)
make install

# Build the Pi application
cd /project/boinc_tutorial/src
make pi_compute
```

### 3. Create BOINC Project
```bash
export USER=root
cd /project/boinc_source/tools
yes | ./make_project --delete_prev_inst --drop_db_first \
    --project_root /project/boinc_project \
    --url_base http://apache/boinc_project/ \
    --db_host db \
    --db_user boincadm \
    --db_passwd boinc123 \
    --project_host localhost \
    boinc_project
```

### 4. Create Templates
**CRITICAL:** These must exist for `create_work` to function.
```bash
mkdir -p /project/boinc_project/templates

cat > /project/boinc_project/templates/pi_in.xml << 'EOF'
<file_info>
    <number>0</number>
</file_info>
<workunit>
    <file_ref>
        <file_number>0</file_number>
        <open_name>in</open_name>
    </file_ref>
    <rsc_fpops_est>1000000000000</rsc_fpops_est>
    <rsc_fpops_bound>10000000000000</rsc_fpops_bound>
    <rsc_memory_bound>100000000</rsc_memory_bound>
    <rsc_disk_bound>100000000</rsc_disk_bound>
    <delay_bound>86400</delay_bound>
</workunit>
EOF

cat > /project/boinc_project/templates/pi_out.xml << 'EOF'
<file_info>
    <name><OUTFILE_0/></name>
    <generated_locally/>
    <upload_when_present/>
    <max_nbytes>1000000</max_nbytes>
    <url><UPLOAD_URL/></url>
</file_info>
<result>
    <file_ref>
        <file_name><OUTFILE_0/></file_name>
        <open_name>out</open_name>
    </file_ref>
</result>
EOF
```

### 5. Deploy Pi Application
```bash
mkdir -p /project/boinc_project/apps/pi_compute/1.0/x86_64-pc-linux-gnu
cp /project/boinc_tutorial/src/pi_compute /project/boinc_project/apps/pi_compute/1.0/x86_64-pc-linux-gnu/
cp /project/boinc_tutorial/src/seal_public.key /project/boinc_project/download/

# Register in Database
mysql -u root -prootpass123 -h db boinc_project -e "
INSERT IGNORE INTO app (name, user_friendly_name, create_time) VALUES ('pi_compute', 'PI Computation', UNIX_TIMESTAMP());
INSERT IGNORE INTO platform (name, user_friendly_name, deprecated, create_time) VALUES ('x86_64-pc-linux-gnu', 'Linux x86-64', 0, UNIX_TIMESTAMP());
"

# Update Versions and Permissions
cd /project/boinc_project
yes | ./bin/update_versions
chown -R www-data:www-data /project/boinc_project
chmod -R 775 /project/boinc_project
chmod +x /project/boinc_project/cgi-bin/*
```

## Step 3: Configure Apache (From Host Machine)
```bash
docker compose exec apache a2ensite boinc_project
docker compose exec apache a2enmod cgi
docker compose exec apache apache2ctl graceful
```

## Step 4: Create Work Units
Inside the `backend` container:
```bash
cd /project/boinc_project
# Create input file
echo "10000000" > download/wu_10m.txt

# Create work unit with 3 result instances
./bin/create_work --appname pi_compute --wu_name pi_wu_$(date +%s) \
    --wu_template templates/pi_in.xml \
    --result_template templates/pi_out.xml \
    --target_nresults 3 \
    wu_10m.txt
```

## Step 5: Start Three BOINC Clients
Inside the `backend` container:
```bash
for i in 1 2 3; do
    CLIENT_DIR="/tmp/client_$i"
    mkdir -p "$CLIENT_DIR"
    /usr/bin/boinc \
        --daemon \
        --dir "$CLIENT_DIR" \
        --attach_project http://apache/boinc_project/ \
        --no_gui_rpc
done
```
Note: Use `--attach_project` for newer BOINC versions if `--project_init` is not available.

## Step 6: Verify Results
Wait a few minutes (approx. 2-5 mins) for the clients to finish.

1. **Check Database Status**:
```bash
mysql -u root -prootpass123 -h db boinc_project -e "SELECT name, outcome, validate_state FROM result;"
```

2. **Compare Result Values**:
```bash
grep "Estimated value of PI" /project/boinc_project/upload/*
```
Check that values are slightly different (demonstrates distribution).

3. **Web Interface**: `http://localhost:8080/boinc_project/`
