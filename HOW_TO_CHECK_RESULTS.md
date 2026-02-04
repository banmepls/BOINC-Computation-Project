# How to Check BOINC Computation Results

This guide shows you how to verify and analyze results from your BOINC distributed computing project.

## Quick Status Check

### 1. Check Database for Workunits and Results

```bash
# Check if workunits exist
docker compose exec backend mysql -u root -prootpass123 -h db boinc_project -e "SELECT name, appid, id FROM workunit;"

# Check result status
docker compose exec backend mysql -u root -prootpass123 -h db boinc_project -e "SELECT id, name, server_state, outcome FROM result;"

# Count completed results
docker compose exec backend mysql -u root -prootpass123 -h db boinc_project -e "SELECT COUNT(*) as completed FROM result WHERE server_state=5 AND outcome=1;"
```

**Result States:**
- `server_state=5`: Result uploaded
- `outcome=1`: Success
- `outcome=3`: Client error
- `outcome=5`: Didn't need (redundant computation)

### 2. Check Upload Directory for Result Files

```bash
# List uploaded result files
docker compose exec backend find /project/boinc_project/upload -type f -name "*" -ls

# View a specific result file
docker compose exec backend cat /project/boinc_project/upload/<fanout_dir>/<result_file>
```

## Checking Specific Application Results

### Pi Calculation Results

```bash
# Find Pi computation results
docker compose exec backend find /project/boinc_project/upload -type f -exec grep -l "Estimated pi" {} \;

# View Pi result
docker compose exec backend cat /project/boinc_project/upload/<path_to_file>
```

**Expected Output:**
```
Total points: 100000
Points inside circle: 78539
Estimated pi: 3.141560000000000

--- SEAL ENCRYPTED DATA ---
ENC_TEMP_START
<hex-encoded encrypted temperature>
ENC_TEMP_END
```

### Euler's Constant Results

```bash
# Find Euler results
docker compose exec backend find /project/boinc_project/upload -type f -exec grep -l "Estimated e" {} \;

# View result
docker compose exec backend cat /project/boinc_project/upload/<path_to_file>
```

**Expected Output:**
```
Total trials: 100000
Estimated e: 2.718281828459045

--- SEAL ENCRYPTED DATA ---
ENC_TEMP_START
<hex-encoded encrypted temperature>
ENC_TEMP_END
```

### Ax=b Solver Results

```bash
# Find Ax=b results
docker compose exec backend find /project/boinc_project/upload -type f -exec grep -l "Solution vector" {} \;

# View result
docker compose exec backend cat /project/boinc_project/upload/<path_to_file>
```

**Expected Output:**
```
Monte Carlo Solution for Ax=b:
Solution vector x:
x[0] = 0.333333
x[1] = 0.333333
x[2] = 0.333333

--- SEAL ENCRYPTED DATA ---
ENC_TEMP_START
<hex-encoded encrypted temperature>
ENC_TEMP_END
```

### Collatz Conjecture Results

```bash
# Find Collatz results
docker compose exec backend find /project/boinc_project/upload -type f -exec grep -l "Longest Collatz" {} \;

# View result
docker compose exec backend cat /project/boinc_project/upload/<path_to_file>
```

**Expected Output:**
```
Search range: 1 to 10000
Longest Collatz sequence:
  Starting number: 6171
  Sequence length: 262
```

## Decrypting and Analyzing Temperature Data

### Run the Temperature Processor

```bash
# Process all results and decrypt temperatures
docker compose exec backend /project/boinc_tutorial/src/process_results \
    /project/boinc_project/upload \
    /project/boinc_tutorial/src/seal_secret.key \
    70.0
```

**Expected Output:**
```
Processed result from pi_wu_1_0: 45.23 C
Processed result from axb_wu_1_0: 47.89 C
-------------------------------------------
Number of readings: 2
Average Temperature: 46.56 C
Threshold: 70.0 C
Temperature within safe range.
-------------------------------------------
```

### Custom Threshold

```bash
# Set custom temperature threshold (e.g., 50°C)
docker compose exec backend /project/boinc_tutorial/src/process_results \
    /project/boinc_project/upload \
    /project/boinc_tutorial/src/seal_secret.key \
    50.0
```

## Database Queries for Detailed Analysis

### View All Applications

```bash
docker compose exec backend mysql -u root -prootpass123 -h db boinc_project -e \
    "SELECT id, name, user_friendly_name FROM app;"
```

### Check Workunit Status

```bash
docker compose exec backend mysql -u root -prootpass123 -h db boinc_project -e \
    "SELECT w.name, a.name as app_name, w.canonical_resultid, w.error_mask 
     FROM workunit w 
     JOIN app a ON w.appid=a.id;"
```

### View Result Details

```bash
docker compose exec backend mysql -u root -prootpass123 -h db boinc_project -e \
    "SELECT r.name, r.server_state, r.outcome, r.cpu_time, r.elapsed_time, w.name as workunit 
     FROM result r 
     JOIN workunit w ON r.workunitid=w.id 
     ORDER BY r.id DESC 
     LIMIT 10;"
```

### Check Client Activity

```bash
docker compose exec backend mysql -u root -prootpass123 -h db boinc_project -e \
    "SELECT id, domain_name, total_credit, expavg_credit, create_time 
     FROM host 
     ORDER BY id DESC 
     LIMIT 5;"
```

## Verification Checklist

- [ ] **Workunits Created**: Check database for workunit entries
- [ ] **Results Uploaded**: Verify files exist in upload directory
- [ ] **Pi Accuracy**: Estimated π should be close to 3.14159
- [ ] **Euler Accuracy**: Estimated e should be close to 2.71828
- [ ] **Ax=b Solution**: Solution vector should satisfy the equation
- [ ] **Collatz Results**: Longest sequence found in range
- [ ] **Temperature Data**: Encrypted data present in Pi and Ax=b results
- [ ] **Decryption Works**: `process_results` successfully decrypts temperatures
- [ ] **Average Calculated**: Average temperature computed across all clients

## Troubleshooting

### No Results in Upload Directory

```bash
# Check if clients are attached
docker compose exec backend ls -la /project/boinc_test/clients/c1/

# Check client logs
docker compose exec backend tail -n 100 /project/boinc_test/clients/c1/stdoutdae.txt

# Check server daemons are running
docker compose exec backend ps aux | grep -E "feeder|transitioner|file_deleter"
```

### Results Not Validated

```bash
# Check validator daemon
docker compose exec backend mysql -u root -prootpass123 -h db boinc_project -e \
    "SELECT name, need_validate FROM workunit WHERE need_validate > 0;"

# Manually run validator (if needed)
docker compose exec backend /project/boinc_project/bin/sample_trivial_validator -d 3
```

### Temperature Decryption Fails

```bash
# Verify secret key exists
docker compose exec backend ls -la /project/boinc_tutorial/src/seal_secret.key

# Check result file format
docker compose exec backend grep -A 5 "ENC_TEMP_START" /project/boinc_project/upload/<path>

# Test with single file
docker compose exec backend bash -c "
    echo 'Testing single file...'
    cat /project/boinc_project/upload/<path> | grep -A 1 ENC_TEMP_START
"
```

## Accessing Results via Web Interface

If Apache is properly configured, you can also view results through the web interface:

```
http://localhost:8080/boinc_project/
```

Navigate to:
- **Results**: View all computation results
- **Workunits**: See pending and completed work
- **Hosts**: Check connected clients
- **Applications**: View registered apps

## Export Results for Analysis

```bash
# Export all Pi results to CSV
docker compose exec backend bash -c "
    mysql -u root -prootpass123 -h db boinc_project -e '
        SELECT r.name, r.cpu_time, r.elapsed_time, r.received_time 
        FROM result r 
        JOIN workunit w ON r.workunitid=w.id 
        JOIN app a ON w.appid=a.id 
        WHERE a.name=\"pi_compute\" AND r.outcome=1
    ' --batch --skip-column-names > /project/pi_results.csv
"

# View exported file
docker compose exec backend cat /project/pi_results.csv
```

## Summary

The main ways to check results are:

1. **Database Queries**: Check workunit and result status
2. **Upload Directory**: View raw result files
3. **Temperature Processor**: Decrypt and analyze encrypted data
4. **Web Interface**: Browse results visually (if Apache working)
5. **Log Files**: Monitor client and server activity

For the most comprehensive check, run the temperature processor after results are uploaded to verify both computation accuracy and encrypted temperature monitoring.
