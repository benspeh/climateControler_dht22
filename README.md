# climate_controle_d22
## d22 on blynk cloud ubuntu core 24 raspberry pi model 3B+ 

### Install
snap install docker

cd /home/git-repositories/

sudo docker run --rm -v "$PWD":/repo -w /repo alpine/git clone https://github.com/benspeh/PiCoolFanService.git
