#!/usr/bin/bash

# Use grouped output messages
infobegin() {
	echo "::group::${1}"
}
infoend() {
	echo "::endgroup::"
}

# Required packages on Ubuntu
requires=(
	ccache # Use ccache to speed up build
)

requires+=(
	autoconf-archive
	autopoint
	git
	libcairo2-dev
	libglib2.0-dev
	libgtk-3-dev
	libmate-panel-applet-dev
	libnotify-dev
	libsensors4-dev
	libtool
	libxml-parser-perl
	libxnvctrl-dev
	make
	mate-common
	yelp-tools
)

infobegin "Update system"
apt-get update -y
infoend

infobegin "Install dependency packages"
env DEBIAN_FRONTEND=noninteractive \
	apt-get install --assume-yes \
	${requires[@]}
infoend
