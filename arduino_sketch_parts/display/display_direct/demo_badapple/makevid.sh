#!/bin/sh

# Download original video
youtube-dl https://www.nicovideo.jp/watch/sm8628149 -o badapple.mp4

# Split into frames
mkdir -p frames
ffmpeg -i "badapple.mp4" -vf scale=20:16  frames/%04d.png

# (optional) Apply dithering (original: https://gist.github.com/lordastley/5127027)
# mkdir ditheredframes
# for i in frames/*.png; do convert $i pgm:- | pamditherbw -atkinson | pnmtopng > dithered$i; done;

# Convert
python convert.py frames/
# python convert.py ditheredframes/
