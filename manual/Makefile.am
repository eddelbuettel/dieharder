# 
# Id: Makefile,v 1.17 2006/02/20 05:11:25 rgb Exp $
#
#======================================================================
# THIS IS A "GENERIC" MAKEFILE FOR LATEX PROJECTS
#
# make          alone makes only the $(DVI) file, running latex twice
# so that all references and toc's are correct.
#
# make ps       makes $(PS) (and $(DVI) if necessary)
# make pdf      makes $(PDF) (and $(PS) and $(DVI) as necessary)
# make html     makes $(HTML) via latex2html, I hope...
# make clean    deletes all transient build files and output formats.
# make tar      makes $(TAR) from >>all<< the sources
# make gz       makes $(TARGZ) from >>all<< the sources
# make svn      does a svn commit and creates the timestamp $(SVN)
# make sync     does a svn commit and runs syncsvn to list of
#               SVNROOT hosts
# 
#=======================================================================
# These are quite nonstandard, as this is a latex project embedded in
# a C project.  Useful template, at that.
SOURCE = dieharder.tex
VERSION = 0.1
RELEASE = 1

SVNTREE = $(HOME)/Src/svn-tree
SVNPATH = $(SVNTREE)/dieharder
SVNTIME = $(HOME/Src/dieharder/dieharder.svn.time)

# The list of includes are things required for the build to work.  Examples

# might be included .tex or .sty files, embedded .eps files, and so forth.
# Naturally, anything on this list should be in the SVN tree.
FIGS = $(shell ls *.fig 2>&1 | grep -v "No such file or directory" | sed -e "/\/bin\/ls:/d")
FEPS = $(FIGS:.fig=.eps)
INCLUDE = $(FIGS)

# Source(s) and production objects.
SOURCES = $(SOURCE) $(INCLUDE) $(FEPS)
LOGS = $(SOURCE:.tex=.log)
TOC = $(SOURCE:.tex=.toc)
AUX = $(SOURCE:.tex=.aux)
ERR = $(SOURCE:.tex=.err)
DVI = $(SOURCE:.tex=.dvi)
PS = $(SOURCE:.tex=.ps)
PDF = $(SOURCE:.tex=.pdf)
A4PS = $(SOURCE:.tex=.a4.ps)
A4PDF = $(SOURCE:.tex=.a4.pdf)
HTMLDIR = $(SOURCE:.tex=)

# Build targets (from rules)
all:	$(PDF)
ps:	$(DVI) $(PS)
pdf:	$(DVI) $(PS) $(PDF)
a4ps:	$(DVI) $(A4PS)
a4pdf:	$(DVI) $(A4PS) $(A4PDF)
tar:	$(TAR)
tgz:	$(TGZ)

# Build targets (from commands)
html:	$(SOURCES)
	latex2html -local_icons $(SOURCE)

$(HTMLDIR):
	make html

#
# These should run in the doc directory, although a commit one level up
# will work too...
#
svn:
	echo "New Checkin `date`" >> $(SVNTIME)	# Will force a commit and increment revision
	svn commit .
	cat $(SVNTIME) | \
	sed -e '/^New Checkin/d' >> $(SVNTIME).tmp
	mv $(SVNTIME).tmp $(SVNTIME)

sync:
	echo "New Checkin `date`" >> $(SVNTIME)	# Will force a commit and increment revision
	svn commit .		# Do the commit
	rsync -avz --delete $(SVNPATH) login.phy.duke.edu:/home/einstein/prof/rgb/Src/svn-tree
	rsync -avz --delete $(SVNPATH) 209.42.212.5:$(SVNTREE)
	cat $(SVNTIME) | \
	sed -e '/^New Checkin/d' >> $(SVNTIME).tmp
	mv $(SVNTIME).tmp $(SVNTIME)

# Clean up all the junk build files and output formats to save space.
clean:
	rm -f $(DVI) $(AUX) $(LOGS) $(TOC) $(ERR) $(PS) $(PDF)

install: $(PDF)
	cp $(PDF) ..

#
#                RULES FOR BUILDING STANDARD TARGETS
#
# Rule for making dvi out of tex.  Do it twice to build table of contents
# and resolve forward references.
%.dvi:%.tex $(SOURCES)
	latex $< $@
	latex $< $@

# Rule for making postscript image
%.ps:%.dvi
	dvips -Pamz -Pcmz -o $@ $<

# Rule for making pdf image
#%.pdf:%.ps
#	ps2pdf $<
%.pdf:%.tex $(SOURCES)
	pdflatex $< $@
	pdflatex $< $@

# Rule for making postscript image
%.a4.ps:%.dvi
	dvips -Pamz -Pcmz -t A4 -o $@ $<

# Rule for making pdf image
%.a4.pdf:%.a4.ps
	ps2pdf $<

# Rule for making .fig->.eps
%.eps:%.fig
	fig2dev -L eps $< > $@

