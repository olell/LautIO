# This file is part of LautIO
# 
# Copyright 2021 - olel
# LautIO is licensed under MIT License
# See LICENSE file for more information
# 
# 

# Sass compiler
SCSSC=sass
SCSSC_FLAGS=
SCSS_MAIN=stylesheets/main.scss

all: clean
	$(SCSSC) $(SCSSC_FLAGS) $(SCSS_MAIN) css/main.css

clean:
	rm -f css/main.css

host: all
	python3 -m http.server 8080
