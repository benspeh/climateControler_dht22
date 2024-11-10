# climate_controle_d22
## d22 on blynk cloud ubuntu core 24 raspberry pi model 3B+ 

### Install blynk-library and dh22 lib
#### install docker
snap install docker
#### create dir
#mkdir -p /home/"your-git-code-directory-here"

#### change dir 
cd /home/git-repositories/ #"your-git-code-directory-here"

#### clone [https://github.com/blynkkk/blynk-library](https://github.com/blynkkk/blynk-library)
sudo docker run --rm -v "$PWD":/repo -w /repo alpine/git clone https://github.com/blynkkk/blynk-library.git

