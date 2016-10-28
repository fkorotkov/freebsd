#!/bin/sh

# This is a hacky script to identify GPL-, permissive-, and dual-licensed
# devicetree files. Results should be manually verified, and quality of
# results of running this against other source trees is unknown.

# multi-line grep global $file for text, with ' ' representing any whitespace
# (including newline) or * (from a beginning-of-line comment marker).
g()
{
	exp=$(echo $1 | sed 's/ /[\\s*]+/g')
	#echo "exp: $exp"
	/usr/local/bin/grep -Pzqi "$exp" $file
}

find ${1:-.} -type f | while read file; do
	file=${file#./}
	if g "this file is dual[-\s]licensed"; then
		if g "the GPL or the X11 license"; then
			if g "GNU General Public License version 2"; then
				echo -n "dual-GPLv2-X11 "
			elif g "either version 2" && g "any later version"; then
				echo -n "dual-GPLv2+-X11 "
			elif g "version 2 of the license"; then
				echo -n "dual-GPLv2-X11 "
			else
				echo -n "dual-GPLv?-X11 "
			fi
		else
			echo -n "dual-unknown "
		fi
	elif g "released using a dual license strategy" && \
	    g "3-clause BSD License" && \
	    g "GNU General Public License Version 2.0"; then
		echo -n "dual-GPLv2-BSD "
	elif g "Licensed under the X11 license or the GPL v2 .or later."; then
		echo -n "dual-GPLv2-X11 "
	elif g "Licensed under GPLv2 or later"; then
		echo -n "GPLv2+ "
	elif g "terms of the GNU General Public License"; then
		if g "either version 2" && g "any later version"; then
			echo -n "GPLv2+ "
		elif g "version 2\\."; then
			echo -n "GPLv2 "
		else
			echo -n "GPLv? "
		fi
	elif g "GPLv2+ "; then
		echo -n "GPLv2 "
	elif g "Licensed under GPLv2\\."; then
		echo -n "GPLv2 "
	elif g "the GNU General Public License, version 2, as published"; then
		echo -n "GPLv2 "
	elif g "GNU General Public License .GPL.,? version 2"; then
		echo -n "GPLv2 "
	elif g "the terms of version 2 of the GNU General Public License as published"; then
		echo -n "GPLv2 "
	elif g "licensed under the GNU/GPL"; then
		echo -n "GPLv? "
	elif g "licensed under the GNU General Public License"; then
		echo -n "GPLv? "
	elif g "Redistributions of source code" && \
	    g "Redistributions in binary form"; then
		echo -n "BSD "
	elif g "The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software"; then
		echo -n "MIT "
	elif g "copyright notice and this permission notice appear in all copies"; then
		echo -n "ISC "
	elif g "copyright"; then
		echo -n "copyright? "
	else
		echo -n "? "
	fi
	echo $file
done
