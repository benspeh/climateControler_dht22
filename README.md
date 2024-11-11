# climate_controle_d22
## d22 on blynk cloud ubuntu core 24 raspberry pi model 3B+ 

### Install blynk-library and dh22 lib
#### install docker
snap install docker
#### create dir
#mkdir -p /home/"your-git-code-directory-here"

#### change dir 
cd /home/git-repositories/ #"your-git-code-directory-here"

#### install WiringPi
sudo docker run --rm -v "$PWD":/repo -w /repo alpine/git clone https://github.com/WiringPi/WiringPi.git

cd WiringPi

udo docker run --rm -v "$PWD":/workspace -w /workspace ubuntu bash -c "apt update && apt install -y build-essential && ./build"

#### clone [https://github.com/blynkkk/blynk-library](https://github.com/blynkkk/blynk-library)
sudo docker run --rm -v "$PWD":/repo -w /repo alpine/git clone https://github.com/blynkkk/blynk-library.git

#### build on raspberry
Follow instructions on https://github.com/blynkkk/blynk-library/tree/master/linux#readme
