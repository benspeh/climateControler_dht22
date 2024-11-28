sudo docker run --rm -v "$PWD":/repo -w /repo alpine/git reset --hard && sudo docker run --rm -v "$PWD":/repo -w /repo alpine/git pull origin main

sudo chmod +x ./serviceFiles/setup_readDHT22_service.sh
sudo chmod +x ./update_climateControler_dht22.sh

sudo docker run --rm \
  --privileged \
  -v "$PWD":/workspace \
  -w /workspace \
  ubuntu bash -c "apt update && apt install -y build-essential make && cd ./pi_readDHT && make clean && make && cd ../ && cd ./climateModel_VPD && make clean && make"


cd ./serviceFiles
sudo ./update_readDHT22_service.sh
cd ../
