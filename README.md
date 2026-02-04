# BOINC-Computation-Project


1. Clonare repositoriu

git clone https://github.com/cherubimro/boinc_tutorial.git


2. Cream Docker Compose si Dockerfile


3. Pornim Docker

docker compose up -d --build


4. Conectam terminalul la container

docker compose exec backend bash


5. Instalam dependente lipsa necesare scripturilor

apt update
apt install -y dos2unix
apt install -y nlohmann-json3-dev
apt install -y libzip-dev


6. Convertim scripturile .sh

cd boinc_tutorial/scripts/
dos2unix *.sh
chmod +x *.sh


7. Pentru rezolare dependente Python pe Debian

rm -f /usr/lib/python*/EXTERNALLY-MANAGED


8. Rulam scripturile

./install_dependencies_debian.sh
./build_boinc.sh


9.

cd /root/boinc_source

