sudo cp readDHT22.service /etc/systemd/system/readDHT22.service

sudo chmod +x /etc/systemd/system/readDHT22.service

sudo systemctl daemon-reload
sudo systemctl enable dynamicI2Cooling.service
sudo systemctl start dynamicI2Cooling.service
sudo systemctl status dynamicI2Cooling.service
