#
## Poorman's Standard-Emulator by GoodKook, goodkook@gmail.com
#	Makefile for QFlow of sample project "signed_multiplier", target tech. ETRI 0.5um CMOS
#

VERILOG_SRCS	=	./source/signed_mult8.v \
					./source/signed_multiplier.v
TOP_MODULE		=	signed_mult8

all :
	@echo 'Makefile for "signed_multiplier" QFlow RTL-to-Layout using ETRI 0.5um CMOS Technology'
	@echo '  Poorman`s Std. Co-Emulation(PSCE), GoodKook, goodkook@gmail.com'
	@echo ''
	@echo 'Usage:'
	@echo '    make [option]'
	@echo '         Use one of following options'
	@echo '             synthesize'
	@echo '             place'
	@echo '             route'
	@echo '             migrate'
	@echo '             lvs'

# -----------------------------------------------------
synthesize : ./log/synth.log

./log/synth.log : $(VERILOG_SRCS)
	qflow synthesize -T etri050 $(TOP_MODULE)

# -----------------------------------------------------
place : ./log/place.log

./log/place.log : \
	./layout/$(TOP_MODULE).par \
	./layout/$(TOP_MODULE).cel2 \
	./log/synth.log \
	project_vars.sh
	qflow place -T etri050 $(TOP_MODULE)

# -----------------------------------------------------
route : ./log/route.log

./log/route.log : \
	./log/place.log \
	./layout/$(TOP_MODULE).def \
	./layout/$(TOP_MODULE)_unroute.def \
	project_vars.sh
	qflow route -T etri050 $(TOP_MODULE)

# -----------------------------------------------------
migrate : ./log/magic_db.log project_vars.sh

./log/magic_db.log : \
	./log/route.log
	qflow migrate -T etri050 $(TOP_MODULE)

# -----------------------------------------------------
lvs : ./log/lvs.log project_vars.sh

./log/lvs.log : \
	./synthesis/$(TOP_MODULE).spc \
	./layout/$(TOP_MODULE).mag \
	./log/magic_db.log
	~/ETRI050_DesignKit/scripts/fix_net_name.sh ./synthesis/$(TOP_MODULE).spc
	qflow lvs -T etri050 $(TOP_MODULE)

clean :
	rm -f ./log/*
	rm -f ./synthesis/*
