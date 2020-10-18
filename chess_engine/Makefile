all: build run

build:
	g++ testEngine.cpp piece.cpp move.cpp helper.cpp chess_map.cpp move_chooser.cpp -std=c++1z -Wall -Wextra -o localEngine
runAdi: build
	../../xboard-4.8.0/./xboard -fcp "/home/student/Desktop/proiectPA/l2-pa-project-BUM/localEngine"
DanRun:
	/home/ursu/workspace/proiect_pa_bun/xboard-4.8.0/./xboard -fcp "/home/ursu/workspace/proiect_pa_bun/l2-pa-project-BUM/localEngine"

run_andrei: 
	/home/andrei/Documents/workspace/PA/tema1/xboard-4.8.0/./xboard -fcp "/home/andrei/Documents/workspace/PA/proiect/l2-pa-project-BUM/localEngine"

allAdi: build runAdi
allAndrei: build run_andrei

runDanFairy:
	/home/ursu/workspace/proiect_pa_bun/xboard-4.8.0/./xboard -fcp "/home/ursu/workspace/proiect_pa_bun/l2-pa-project-BUM/localEngine" -scp "fairymax" -secondInitString "new\nrandom\nsd 2\n" -tc 5 -inc 2 -mg 1 

runAndreiFairy:
	/home/andrei/Documents/workspace/PA/tema1/xboard-4.8.0/./xboard -fcp "/home/andrei/Documents/workspace/PA/proiect/l2-pa-project-BUM/localEngine" -scp "fairymax" -secondInitString "new\nrandom\nsd 2\n" -tc 5 -inc 2 -mg 1 
	
runAdiFairy:
	../../xboard-4.8.0/./xboard -fcp "/home/student/Desktop/proiectPA/l2-pa-project-BUM/localEngine" -scp "fairymax" -secondInitString "new\nrandom\nsd 2\n" -tc 5 -inc 2 -mg 1 -reuseFirst false 

runAdiFairyRev:
	../../xboard-4.8.0/./xboard -fcp "fairymax" -scp "/home/student/Desktop/proiectPA/l2-pa-project-BUM/localEngine" -firstInitString "new\nrandom\nsd 2\n" -tc 5 -inc 2 -mg 1 

runTheirTest:
	../../xboard-4.8.0/./xboard -fcp "make run" -scp "fairymax" -secondInitString "new\nrandom\nsd 2\n" -tc 5 -inc 2 -autoCallFlag true -mg 10 -sgf partide.txt -reuseFirst false 

run:
	./localEngine
