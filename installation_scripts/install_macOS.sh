#!/bin/bash

# SCRIPT=$(readlink "$0");
# SCRIPTPATH=$(dirname "$SCRIPT");
# cd ${SCRIPTPATH};

Ask user if they would like a user or system installation
echo -e "Choose:\n[0]\tsystem installation (requires super user privileges)\n[1]\tuser installation\n(*)Note: if performing a user installation, you may need to add the path '\${HOME}\Library\Frameworks' to the environment variable 'DYLD_FALLBACK_FRAMEWORK_PATH' (in case you encounter linker errors when executing the examples) \n\n";
read choice;

if [[ choice -eq 0 ]]; then
	INSTALL_PATH=/usr/local/lib/Tsukuyomi;
	SDL_INSTALL_PATH=/Library/Frameworks;
elif [[ choice -eq 1 ]]; then
	INSTALL_PATH=${HOME}/Library/Tsukuyomi;
	SDL_INSTALL_PATH=${HOME}/Library/Frameworks;
else
	echo "Please choose an appropriate installation (0 or 1)...";
	exit $?;
fi;

# must also look for audio and mixer frameworks
if [[ !( -d "$SDL_INSTALL_PATH/SDL2.framework" ) ]]; then
	echo "SDL2 dependency not found, attempting install into" $SDL_INSTALL_PATH;
	curl "https://www.libsdl.org/release/SDL2-2.0.18.dmg" --output SDL2-2.0.18.dmg;
	hdiutil mount SDL2-2.0.18.dmg &> /dev/null;
	cp -r /Volumes/SDL2/SDL2.framework "$SDL_INSTALL_PATH/";
	hdiutil unmount /Volumes/SDL2;
	mv SDL2-2.0.18.dmg ${HOME}/Downloads;
	echo -e "SDL2 installation successful!\n(moved SDL2 installer moved into ${HOME}/Downloads)\n\n";
fi;

if [[ !( -d "$SDL_INSTALL_PATH/SDL2_image.framework" ) ]]; then
	echo "SDL2-image dependency not found, attempting install into" $SDL_INSTALL_PATH;
	curl "https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.5.dmg" --output SDL2_image-2.0.5.dmg;
	hdiutil mount SDL2_image-2.0.5.dmg &> /dev/null;
	cp -r /Volumes/SDL2_image/SDL2_image.framework "$SDL_INSTALL_PATH/";
	hdiutil unmount /Volumes/SDL2_image;
	mv SDL2_image-2.0.5.dmg ${HOME}/Downloads;
	echo -e "SDL2-image installation successful!\n(moved SDL2-image installer moved into ${HOME}/Downloads)\n\n";
fi;

if [[ !( -d "$SDL_INSTALL_PATH/SDL2_ttf.framework" ) ]]; then
	echo "SDL2-ttf dependency not found, attempting install into" $SDL_INSTALL_PATH;
	curl "https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-2.0.18.dmg" --output SDL2_ttf-2.0.18.dmg;
	hdiutil mount SDL2_ttf-2.0.18.dmg &> /dev/null;
	cp -r /Volumes/SDL2_ttf/SDL2_ttf.framework "$SDL_INSTALL_PATH/";
	hdiutil unmount /Volumes/SDL2_ttf;
	mv SDL2_ttf-2.0.18.dmg ${HOME}/Downloads;
	echo -e "SDL2-ttf installation successful!\n(moved SDL2-ttf installer moved into ${HOME}/Downloads)\n\n";
fi;

if [[ !( -d "$SDL_INSTALL_PATH/SDL2_mixer.framework" ) ]]; then
	echo "SDL2_mixer dependency not found, attempting install into" $SDL_INSTALL_PATH;
	curl "https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-2.0.4.dmg" --output SDL2_mixer-2.0.4.dmg;
	hdiutil mount SDL2_mixer-2.0.4.dmg &> /dev/null;
	cp -r /Volumes/SDL2_mixer/SDL2_mixer.framework "$SDL_INSTALL_PATH/";
	hdiutil unmount /Volumes/SDL2_mixer;
	mv SDL2_mixer-2.0.4.dmg ${HOME}/Downloads;
	echo -e "SDL2_mixer installation successful!\n(moved SDL2_mixer installer moved into ${HOME}/Downloads)\n\n";
fi;

cp Makefiles/Makefile_macOS Makefile;
make;

if [[ choice -eq 0 ]]; then
	sudo mkdir $INSTALL_PATH;
	sudo cp build/Tsukuyomi.a "$INSTALL_PATH/";
	sudo mkdir "$INSTALL_PATH/include";
else
	mkdir $INSTALL_PATH;
	cp build/Tsukuyomi.a "$INSTALL_PATH/";
	mkdir $INSTALL_PATH/include;
fi;

cd src/;
find . -name '*.hpp' | cpio -updm $INSTALL_PATH/include/;
cd ../;
echo -e "Successful installation\nInstall location:" $INSTALL_PATH;
