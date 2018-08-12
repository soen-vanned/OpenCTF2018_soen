
all:
	@gcc -static itanic.c sha256.c aes.c -o itanic -s
	@strip itanic

debug:
	@gcc -static itanic.c sha256.c aes.c -o itanic -ggdb

zip:
	@zip --quiet itanic.zip itanic printer.enc hello_world.enc dock.enc quine.enc LostKng.enc readme.txt &>/dev/null
