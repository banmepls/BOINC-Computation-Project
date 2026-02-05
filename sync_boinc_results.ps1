#!/bin/bash
# Script pentru sincronizare BOINC results

echo "[$(date)] Starting BOINC results sync..."

# 1. Copiază fișierele din backend dacă există
if docker compose exec backend test -d "/project/boinc_project/upload"; then
    echo "Copying files from backend..."
    docker compose exec backend find /project/boinc_project/upload -type f -name "*.txt" -o -name "*.xml" -o -name "*.log" | \
    while read file; do
        filename=$(basename "$file")
        docker compose exec backend cat "$file" | docker compose exec -T apache bash -c "cat > /var/www/html/upload/$filename"
    done
fi

# 2. Actualizează index.php dacă nu există
if ! docker compose exec apache test -f "/var/www/html/upload/index.php"; then
    echo "Creating index.php..."
    docker compose exec apache bash -c 'cat > /var/www/html/upload/index.php << "EOF"
<?php
$files = glob("*.*");
echo "<h1>BOINC Results</h1>";
foreach($files as $file) {
    if($file != "index.php") {
        echo "<p><a href=\"$file\">$file</a></p>";
    }
}
?>
EOF'
fi

# 3. Setează permisiuni
docker compose exec apache bash -c "chmod -R 755 /var/www/html/upload"
docker compose exec apache bash -c "chown -R www-data:www-data /var/www/html/upload"

echo "[$(date)] Sync completed. Files in uploads:"
docker compose exec apache bash -c "ls -la /var/www/html/upload/ | wc -l"
