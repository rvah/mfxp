#!/bin/sh
if command -v ctags &> /dev/null
then
	ctags -R
fi
