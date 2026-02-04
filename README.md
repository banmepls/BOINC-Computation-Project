# BOINC-Computation-Project

1. Clonare repositoriu

```bash
git clone https://github.com/cherubimro/boinc_tutorial.git
```

2. Cream Docker Compose si Dockerfile


3. Pornim Docker

```bash
docker compose up -d --build
```

4. Conectam terminalul la container

```bash
docker compose exec backend bash
```

5. Instalam dependente lipsa necesare scripturilor

```bash
apt update
apt install -y dos2unix
apt install -y nlohmann-json3-dev
apt install -y libzip-dev
```

6. Convertim scripturile .sh

```bash
cd boinc_tutorial/scripts/
dos2unix *.sh
chmod +x *.sh
```

7. Pentru rezolare dependente Python pe Debian

```bash
rm -f /usr/lib/python*/EXTERNALLY-MANAGED
```

8. Rulam scripturile

```bash
./install_dependencies_debian.sh
./build_boinc.sh
```

9.

```bash
cd /root/boinc_source
```
