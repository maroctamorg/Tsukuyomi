#!/bin/bash

#Ask user if they would like a user or system installation
#Figure out best directories for static libraries

SCRIPT=$(readlink -f "$0");
SCRIPTPATH=$(dirname "$SCRIPT");
cd ${SCRIPTPATH};

if [[ -d ${HOME}/Library/Frameworks/SDL2.framework ]]
then
	INSTALL_PATH=${HOME}/Library/Frameworks/Tsukuyomi.framework;
elif [[ -d /Library/Frameworks/SDL2.framework ]]
then
	INSTALL_PATH=${HOME}/Library/Frameworks/Tsukuyomi.framework;
	
else
	echo "SDL2 dependency not found, attempting install into" ${HOME}/Library/Frameworks/SDL2.framework
	wget "https://www.libsdl.org/release/SDL2-2.0.18.dmg";
	mount -t SDL2-2.0.18.dmg SDL2; # what type?
	cp SDL2/SDL2.framework ${HOME}/Library/Frameworks/SDL2.framework;
	umount SDL2;
	rm SDL2-2.0.18.dmg;
	INSTALL_PATH=${HOME}/Library/Frameworks/Tsukuyomi.framework;
fi

cp Makefiles/Makefile_macOS Makefile;
make;
mkdir ${INSTALL_PATH};
cp build/Tsukuyomi.a ${INSTALL_PATH}/;
mkdir ${INSTALL_PATH}/include;
cp -r src ${INSTALL_PATH}/include/;

echo -e "Successful installation\nInstall location:" ${INSTALL_PATH};
