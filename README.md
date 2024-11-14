# climate_controle_d22
## d22 on blynk cloud ubuntu core 24 raspberry pi model 3B+ 

### Install blynk-library and dh22 lib
#### install docker
snap install docker
#### create dir
#mkdir -p /home/"your-git-code-directory-here"

#### change dir 
cd /home/git-repositories/ #"your-git-code-directory-here"

#### clone [blynk-library](https://github.com/blynkkk/blynk-library)
sudo docker run --rm -v "$PWD":/repo -w /repo alpine/git clone https://github.com/blynkkk/blynk-library.git

#### clone [WiringPi](https://github.com/WiringPi/WiringPi)
sudo docker run --rm -v "$PWD":/repo -w /repo alpine/git clone https://github.com/WiringPi/WiringPi.git

#### clone [Robust Adafruit DHT22 sensor reader in C++, using wiringPi](https://github.com/Haydeni0/dht22)
sudo docker run --rm -v "$PWD":/repo -w /repo alpine/git clone https://github.com/Haydeni0/dht22.git

/home/git-repositories/climateControle_dht22$ sudo docker run -it   --device /dev/gpiomem:/dev/gpiomem --privileged   -v "$PWD":/workspace -w /workspace   ubuntu bash -c "apt update && apt install -y build-essential sudo && cd ./WiringPi && ./build || true && cd ../ && cd ./blynk-library/linux && ./build.sh raspberry && ./blynk --token="auth"; bash"


#### build on raspberry
Follow instructions on https://github.com/blynkkk/blynk-library/tree/master/linux#readme
