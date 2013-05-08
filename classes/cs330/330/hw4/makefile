#JAVAPATH=/research/java/j2sdk1.4.2/bin/
#CLASSPATH=.:/research/java/j2sdk1.4.2/lib/classes.zip
JOPTIONS=-g
USERHOME=$(HOME)

.SUFFIXES:
.SUFFIXES: .class .java
#.java.class: export CLASSPATH; CLASSPATH=$(CLASSPATH); $(JAVAPATH)/javac -nowarn $(JOPTIONS) $*.java
.java.class: $(JAVAPATH)javac $(JOPTIONS) $*.java

#
# As you define new classes, add them to the following list.
# It may not be absolutely necessary, but it will help guarantee that
# necessary recompilation gets done.
#
TOPPACKAGE=.
CLASSES:=$(patsubst %.java,%.class,$(wildcard *.java))

TARGET=Orbital
ASSIGNMENT=CS330_S11_OrbitalGUI

%.class: %.java
	$(JAVAPATH)javac -nowarn $(JOPTIONS) $*.java


all: $(ASSIGNMENT).jar


run: $(ASSIGNMENT).jar
	$(JAVAPATH)java  -cp $(ASSIGNMENT).jar $(TARGET)


runsample: 
	$(JAVAPATH)java -classpath '/home/zeil/cs330/Assignments/orbitalGUI/$(ASSIGNMENT).jar'  $(TARGET)

jar: $(ASSIGNMENT).jar


$(ASSIGNMENT).jar: $(CLASSES)
	find $(TOPPACKAGE) -name '*.class' | xargs jar cf temp.jar
	$(JAVAPATH)java -cp /home/zeil/src/retroguard/retroguard.jar RetroGuard temp.jar $(ASSIGNMENT).jar retroguard.dat
	rm temp.jar 


applet: $(ASSIGNMENT).jar $(USERHOME)/public_html/$(ASSIGNMENT) index.html
	cp $(ASSIGNMENT).jar $(USERHOME)/public_html/$(ASSIGNMENT)
	chmod 644 $(USERHOME)/public_html/$(ASSIGNMENT)/$(ASSIGNMENT).jar
	cp index.html $(USERHOME)/public_html/$(ASSIGNMENT)/index.html
	chmod 644 $(USERHOME)/public_html/$(ASSIGNMENT)/index.html
	echo Use a web browser to visit http://www.cs.odu.edu/~$(USER)/$(ASSIGNMENT)/

$(USERHOME)/public_html:
	-mkdir $(USERHOME)/public_html
	chmod o+rx $(USERHOME)/public_html

$(USERHOME)/public_html/$(ASSIGNMENT): $(USERHOME)/public_html
	-mkdir $(USERHOME)/public_html/$(ASSIGNMENT)
	chmod o+rx $(USERHOME)/public_html/$(ASSIGNMENT)


clean:
	-rm -f *.class
	-rm -f $(ASSIGNMENT).jar
	-rm -rf $(USERHOME)/public_html/$(ASSIGNMENT)






