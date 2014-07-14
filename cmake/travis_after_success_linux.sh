#!/bin/bash

cd build
cd src

echo "set basedir \".\"" > OpenRaider.ini
echo "set pakdir \".\"" >> OpenRaider.ini
echo "set audiodir \".\"" >> OpenRaider.ini
echo "set datadir \"../../data\"" >> OpenRaider.ini
echo "set font \"../../data/test.ttf\"" >> OpenRaider.ini

xvfb-run -n 99 -s "-screen 0 640x480x24" ./OpenRaider &

# Wait for OpenRaider to start
wget http://imgur.com/tools/imgurbash.sh
chmod a+x ./imgurbash.sh
sleep 10

import -window root -display :99.0 screenshot.png
./imgurbash.sh screenshot.png > url
(
    echo "NICK OR-$(echo -n $TRAVIS_OS_NAME)-$(echo -n $CC)"
    echo "USER OR-$(echo -n $TRAVIS_OS_NAME)-$(echo -n $CC) 9 \* :OR-$(echo -n $TRAVIS_OS_NAME)-$(echo -n $CC)"
    sleep 5
    echo "JOIN #OpenRaider"
    sleep 5
    echo "PRIVMSG #OpenRaider :New Screenshot $(cat url | head -n 1)"
    sleep 5
    echo QUIT
) | nc chat.freenode.net 6667

