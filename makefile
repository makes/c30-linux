all: build

.PHONY : build
build: 
	@scripts/build.sh

.PHONY : install
install:
	@scripts/install.sh
	@scripts/make-bin-links.sh
	@scripts/nonfree.sh

.PHONY : nonfree
nonfree:
	@scripts/nonfree.sh

.PHONY : clean
clean:
	@scripts/clean.sh

