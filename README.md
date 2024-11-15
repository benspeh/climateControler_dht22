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

#### clone https://github.com/yamasy/pi_dht_read.git
    sudo docker run --rm -v "$PWD":/repo -w /repo alpine/git clone https://github.com/yamasy/pi_dht_read.git
    
#### edit bcm2708.c to your pi model (here 3b+)
    #define BASE 0x3F000000          // Base address for the Raspberry Pi 3B+ peripherals
    #define GPIO_BASE (BASE + 0x200000) // GPIO registers base address = 0x3F200000
    #define GPIO_LENGTH 4096         // The GPIO memory-mapped length (4KB)
    #define BCM2837_ST_BASE (BASE + 0x3000)  // BCM2837 System Timer base address (0x3F003000)

#### edit test_dht_read.c to used bcmPin number (here 24)
    #define DHTPIN 24

#### test https://github.com/yamasy/pi_dht_read.git

    sudo docker run -it     --privileged     --device /dev/mem:/dev/mem     --device /dev/gpiomem:/dev/gpiomem     -v "$PWD":/workspace     -w /workspace     ubuntu     bash -c "apt update && apt install -y build-essential sudo && rm test_dht_read  && make && ./test_dht_read; bash"

### edit blynk-library/build.sh 

    #sudo apt-get update
    #sudo apt-get upgrade
    #sudo apt-get install build-essential git-core
    #git clone git://git.drogon.net/wiringPi
    #cd wiringPi
    #git pull origin
    #./build
    #gpio -v
    #cd ..

### edit blynk-library/main.cpp

    #define BLYNK_TEMPLATE_ID           "TMPL4_*******"
    #define BLYNK_TEMPLATE_NAME         "cimateControle"
    #define BLYNK_AUTH_TOKEN "*****************************"

### test blynk-library 

    sudo docker run -it   --device /dev/gpiomem:/dev/gpiomem --privileged   -v "$PWD":/workspace -w /workspace   ubuntu bash -c "apt update && apt install -y build-essential sudo && cd ./WiringPi && ./build || true && cd ../ && cd ./blynk-library/linux && ./build.sh raspberry && ./blynk --token="Auth"; bash"


#### build on raspberry
Follow instructions on https://github.com/blynkkk/blynk-library/tree/master/linux#readme
