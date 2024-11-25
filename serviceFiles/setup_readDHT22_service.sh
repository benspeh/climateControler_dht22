sudo cp readDHT22.service /etc/systemd/system/readDHT22.service

sudo chmod +x /etc/systemd/system/readDHT22.service

sudo systemctl daemon-reload
sudo systemctl enable readDHT22.service
sudo systemctl restart readDHT22.service
sudo systemctl status readDHT22.service
