
sudo systemctl stop dynamicI2Cooling.service

sudo docker run --rm -v "$PWD":/repo -w /repo alpine/git reset --hard
sudo docker run --rm -v "$PWD":/repo -w /repo alpine/git pull origin main

sudo cp readDHT22.service /etc/systemd/system/dynamicI2Cooling.service

sudo chmod +x /etc/systemd/system/dynamicI2Cooling.service
sudo chmod +x ./dynamicI2CoolingRasPi.sh
sudo chmod +x ./update_dynamicI2CoolingService.sh

sudo systemctl daemon-reload
sudo systemctl restart dynamicI2Cooling.service
sudo systemctl status dynamicI2Cooling.service
