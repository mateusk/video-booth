# openFrameworks Video booth application

## Description

This repository contains the source code for applications used in a video recording booth at 'Nirvana: Taking Punk to the Masses', an exhibition which took place in June/2017 at the Bienal Pavillion in São Paulo, Brazil. The video recording booth was produced by [Maxi](http://maxionline.com.br) and Dançar Marketing.

The booth had 3 applications running in parallel: a video recording application, a video player application, and a registration form. The video recording application was used by visitors to record a video of themselves singing a Nirvana song. The video player application was used to play the recorded videos. The registration form was used to register the visitors' participation in the booth, and to add metadata to the recorded videos.

Once the visitor recorded a video, the PHP backend would update a database file, updating the playlist of the video player app. The video player app would then play the videos in the playlist, in a loop.

The video recording and video player applications were developed for Windows 10, using C++/[OpenFrameworks](http://openframeworks.cc), version 0.9.8. Video recording also made use of the [FFMPEG](http://ffmpeg.org) framework.

A HTML form was used to collect visitor data, and PHP scripts generated a database of recorded videos. [XAMPP](https://www.apachefriends.org/index.html) was used to serve the HTTP form pages, run the PHP scripts and the database.

## Hardware

The camera used in the installation was the [Logitech C920](http://www.logitech.com/en-us/product/hd-pro-webcam-c920). The main application is designed for a [Dell All-in-One Inspiron 24 5000](http://www.dell.com/en-us/shop/productdetails/inspiron-24-5488-aio), with a touch screen. The tablet used for the registration application was a [Samsung Galaxy Tab 4](http://www.samsung.com/uk/tablets/galaxy-tab-4-10-1-t530/SM-T530NYKABTU/). For the installation, there was a wireless router connected to the booth PC through a network cable, and the tablet was connected to the router over Wi-Fi.

## Development

The video recording and player apps were developed using Visual Studio Community 2015. Use the provided openFrameworks source code to continue development.

The video booth machine must be running a updated Windows version, and have FFMPEG installed (follow [this guide](http://www.wikihow.com/Install-FFmpeg-on-Windows)).

Source code for the video recording and player apps can be found in the `videoRecorderPlayer/of_v0.9.8_vs_release/apps/myApps/` folder. In order to compile the apps, openFrameworks must be compiled first. 

The video recording application uses a modified version of the `ofxVideoRecorder` addon, which is in the files of this project, so __do not use another version of this addon__.

Due to a bug in openFrameworks, the audio joining of the music chosen by the visitor in the booth with their recorded video takes place through a muxing done by FFMPEG, through a command line statement made by the application itself at the end of the recording.

## Running the apps

A USB camera must be connected to a USB port on the computer before running the app. The default settings for the app are found in the `videoRecorderPlayer/of_v0.9.8_vs_release/apps/myApps/videoRecorder/bin/data/config.xml` file. In this file, configurations for the resolution of the application, resolution of recorded videos, camera system name, among others, must be set.

The files contained in `htmlRegisterForm` have the HTML pages of the registration form, as well as the PHP scripts. The booth PC was configured with the fixed IP of `192.168.0.2`. A Kiosk mode browser was used for the registration form. The browser was configured to open the `http://192.168.0.2/nirvana/1.html` page.
