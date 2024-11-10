# climate_controle_d22
## d22 on blynk cloud ubuntu core 24 raspberry pi model 3B+ 

### Install blynk-library and dh22 lib
snap install docker

#mkdir -p /home/"your-git-code-directory-here"

cd /home/git-repositories/ #"your-git-code-directory-here"

#clone [https://github.com/blynkkk/blynk-library](https://github.com/blynkkk/blynk-library)

sudo docker run --rm -v "$PWD":/repo -w /repo alpine/git clone [https://github.com/benspeh/PiCoolFanService.git](https://github.com/blynkkk/blynk-library.git)
