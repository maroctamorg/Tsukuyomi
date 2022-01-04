#!/bin/bash

SCRIPT=$(readlink "$0");
SCRIPTPATH=$(dirname "$SCRIPT");
cd ${SCRIPTPATH};

#Ask user if they would like a user or system installation
echo -e "Choose:\n[0]\tsystem installation (requires super user privileges)\n[1]\tuser installation";
read choice;

if [[ choice -eq 0 ]]; then
	INSTALL_PATHS=(/usr/local/lib/Tsukuyomi ${HOME}/Library/Tsukuyomi)
	SDL_INSTALL_PATH=/Library/Frameworks/SDL2.framework
elif [[ choice -eq 1 ]]; then
	INSTALL_PATHS=(${HOME}/Library/Tsukuyomi)
	SDL_INSTALL_PATH=${HOME}/Library/Frameworks/SDL2.framework
else
	echo "Please choose an appropriate installation (0 or 1)...";
	exit $?;
fi

# must also look for audio and mixer frameworks
if [[ !( -d ${HOME}/Library/Frameworks/SDL2.framework || -d /Library/Frameworks/SDL2.framework ) ]]; then
	echo "SDL2 dependency not found, attempting install into" SDL_INSTALL_PATH;
	wget "https://www.libsdl.org/release/SDL2-2.0.18.dmg";
	hdiutil mount SDL2;
	cp /Volumes/SDL2/SDL2.framework SDL_INSTALL_PATH;
	hdiutil unmount /Volumes/SDL2 &
	mv SDL2-2.0.18.dmg ${HOME}/Downloads;
fi

cp Makefiles/Makefile_macOS Makefile;
make;
for path in ${INSTALL_PATHS[@]}; do
	mkdir ${path};
	cp build/Tsukuyomi.a ${path}/;
	if [[ choice -eq 0 ]]; then
		sudo mkdir ${path}/include;
	else
		mkdir ${path}/include;
	fi
	cd src/;
	find . -name '*.hpp' | cpio -updm ${path}/include/;
	cd ../
	echo -e "Successful installation\nInstall location:" ${path};
done
