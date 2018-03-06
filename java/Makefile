JAVAC   = javac -source 1.5 -target 1.5
SOURCES = $(wildcard *.java)
CLASSES = $(SOURCES:.java=.class)

all: $(CLASSES) OthelloFramework.jar

OthelloFramework.jar: $(CLASSES)
	jar -cf ../OthelloFramework.jar *.class

%.class: %.java
	$(JAVAC) $<

clean:
	rm -f $(CLASSES) OthelloGame*MoveThread.class OthelloGame*InitThread.class
