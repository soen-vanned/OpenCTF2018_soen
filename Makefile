all: setup challenges

challenges: bbs_challenge talk_to_the_machine_challenge babys_first_rop_challenge reeeeeeeee_challenge itanic_challenge 90s_telco_challenge

setup:
	@mkdir -p tars
	@echo "[OpenCTF - Soen's challenges]"

bbs_challenge:
	@echo "    [bbs]"
	@echo "        [compiling]"
	@arm-linux-gnueabi-gcc -s bbs/bbs.c -o bbs/bbs -Wno-format-security -O0 -march=armv7-a -static -Wl,-Ttext=0x08040000
	@arm-linux-gnueabi-strip -s bbs/bbs
	@echo "        [tar.gz creation]"
	@cd bbs; tar -czf ../bbs.tar.gz bbs flag bbs.config passwords setup.sh bbs_qemu_wrapper
	@mv bbs.tar.gz tars/

talk_to_the_machine_challenge:
	@echo "    [talk to the machine]"
	@echo "        [tar.gz creation]"
	@cd talk_to_the_machine; tar -czf ../talk_to_the_machine.tar.gz talk_to_the_machine.py flag talk_to_the_machine.config setup.sh
	@mv talk_to_the_machine.tar.gz tars/

babys_first_rop_challenge:
	@echo "    [babys_first_rop]"
	@echo "        [compiling]"
	@gcc babys_first_rop/babys_first_rop.c -o babys_first_rop/babys_first_rop -fno-stack-protector
	@echo "        [tar.gz creation]"
	@cd babys_first_rop; tar -czf ../babys_first_rop.tar.gz babys_first_rop flag babys_first_rop.config setup.sh
	@mv babys_first_rop.tar.gz tars/

reeeeeeeee_challenge:
	@echo "    [reeeeeeeee]"
	@echo "        [compiling]"
	@sparc64-linux-gnu-gcc -static reeeeeeeee/reeeeeeeee.c -o reeeeeeeee/reeeeeeeee
	@echo "        [tar.gz creation]"
	@cd reeeeeeeee; tar -czf ../reeeeeeeee.tar.gz reeeeeeeee flag reeeeeeeee.config setup.sh reeeeeeeee_wrapper
	@mv reeeeeeeee.tar.gz tars/

90s_telco_challenge:
	@echo "    [90s_telco]"
	@echo "        [compiling]"
	@gcc 90s_telco/90s_telco.c -o 90s_telco/90s_telco -fno-stack-protector -static
	@echo "        [tar.gz creation]"
	@cd 90s_telco; tar -czf ../90s_telco.tar.gz 90s_telco flag
	@mv 90s_telco.tar.gz tars/

itanic_challenge:
	@echo "    [itanic]"
	@echo "        [compiling]"
	@gcc -static -s itanic/itanic.c itanic/sha256.c itanic/aes.c -o itanic/itanic
	@strip itanic/itanic
	@echo "        [tar.gz creation]"
	@cd itanic; tar -czf ../itanic.tar.gz itanic flag itanic.config setup.sh printer.enc hello_world.enc dock.enc quine.enc LostKng.enc readme.txt
	@mv itanic.tar.gz tars/
	@echo "        [zip creation]"
	@cd itanic; zip --quiet itanic.zip printer.enc hello_world.enc dock.enc quine.enc LostKng.enc readme.txt

clean:
	@rm -rf tars/*
