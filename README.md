# BOINC-Computation-Project

1. Pornim Docker

```bash
docker compose up -d --build
```

2. Conectam terminalul la container

```bash
docker compose exec backend bash
```

3. Instalam dependente lipsa necesare scripturilor

```bash
apt update
apt install -y dos2unix
apt install -y nlohmann-json3-dev
apt install -y libzip-dev
```

4. Convertim scripturile .sh

```bash
cd boinc_tutorial/scripts/
dos2unix *.sh
chmod +x *.sh
```

5. Pentru rezolare dependente Python pe Debian

```bash
rm -f /usr/lib/python*/EXTERNALLY-MANAGED
```

6. Rulam scripturile

```bash
./install_dependencies_debian.sh
./build_boinc.sh
```

7.
