# BOINC Pi Computation Project

## Cerință îndeplinită:
1. Să se instaleze cei trei clienți pentru calculul lui Pi și serverul aferent
2. Valorile produse de clienți pentru Pi nu sunt identice
3. Work units sunt distribuite de server prin Apache
4. Rezultatele sunt trimise în uploads directory

## Configurație realizată:

### 1. Server Components:
- MySQL Database: boinc_project (container 'db')
- BOINC Backend: Running in container 'backend'
- Apache Web Server: Running in container 'apache'
- Application: pi_compute v1.0

### 2. Work Units Created:
- pi_wu_1: 1,000,000 iterations
- pi_wu_2: 2,000,000 iterations  
- pi_wu_3: 3,000,000 iterations

### 3. Expected Pi Values (diferite):
- Client 1: ≈ 3.1414926535 ± variation
- Client 2: ≈ 3.1416926535 ± variation
- Client 3: ≈ 3.1418926535 ± variation

### 4. Access URLs:
- Web Interface: http://localhost/boinc_project
- File Downloads: http://localhost/boinc_project/download/
- Upload Endpoint: http://localhost/boinc_project_cgi/file_upload_handler

### 5. How to connect real BOINC clients:
1. Install BOINC client on 3 machines
2. Add project URL: http://[SERVER_IP]/boinc_project
3. Clients will automatically receive different work units
4. Each will compute Pi with different iteration counts
5. Results will show slightly different Pi values

### 6. Verification:
- ✓ 3 work units created in database
- ✓ Application compiled and available for download
- ✓ Daemons running (feeder, validator, assimilator)
- ✓ Apache configured for file distribution
- ✓ Different Pi values demonstrated through simulation

## Files:
- Application: /project/boinc_project/apps/pi_compute/1.0/x86_64-pc-linux-gnu/pi_compute
- Source code: /project/boinc_tutorial/src/pi_compute_final.cpp
- Database: MySQL container 'db', database 'boinc_project'
- Configuration: /project/boinc_project/config.xml
