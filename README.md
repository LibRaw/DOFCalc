# DOFCalc v0.2
## README

This is my first application for mobile phones, and my first on Qt+QML.

When I started studying QML, I decided that it might be useful to write a small but complete program that I will use myself.
Aside from this, the DOF calculators that I found for mobile platforms seemed very overloaded: when I'm out shooting, I need to quickly input three parameters (focal length, aperture, distance) and get the result (hyperfocal distance, near and far limits of acceptable sharpness). I don't want to see anything else; this is where the idea for the program was born.

It has no (common in the modern day) bells and whistles, such as
 * setting the circle of confusion by choosing from a million options ("Nikon, APS-C", "Canon APS-H"...), the circle is set directly, in microns
 * teleconverter support (meaning automatic re-calculation of lens speed and focal length).

## Notes
When I had a working prototype ready, I noticed that there did not turn out to be too much code, and decided, out of sporting interest, to try to use as little code as possible with minimal losses in readability. 
For this reason:
 * the braces are set differently than I usually do, but that minimizes the number of lines,
 * any repetition was mercilessly generalized, there are almost no repeated pieces of code,
 * data models are designed based on ideas of compactness, not readability,
With these tricks I was able to keep entire project below 600 lines (before copyright statements were added), this is good enough for some really useful project.

## TODO
 * I'm not happy with automated font size selection code, it works under Android (any reasonable device screen size), but not on Windows. It should be some more elegant way to set default font size in QML (any advices are welcome).
 * Build/deploy files are not included. Only code and minimal qmake .pro file.

The project is not done yet: there are other things a photographer (more precisely, I myself) needs in the field; so the work is to be quietly continued.

## Licensing 
DOFCalc is (C) Copyright 2019 by Alex Tutubalin, LibRaw LLC, lexa@libraw.org. All Right Reserved

DOFCalc is distributed under the terms of GNU General Public License v3

Alex Tutubalin, LibRaw LLC, Mar 28, 2019
