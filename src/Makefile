test:
	g++-4.8 -g -std=c++11 testFlowMP.cpp -lboost_thread -lboost_system -lstdc++ -lm -I../ -I../include -I../src -o testExec

testOther:
	g++-4.8 -std=c++11 -lboost_thread -lboost_system -I../include -I../src testFlowMP.cpp -o testExec -v

testBoost1_49:
	g++ -std=c++11 -lboost_thread -lboost_system -I../include -I../src -I../boost_1_49_0/stage/lib -I../boost_1_49_0/boost testFlowMP.cpp -o testExec -v

test_bm:
		g++-4.8 -g -std=c++11 mainPD_bm.cpp flowMP.cpp graph.cpp -lboost_timer -lboost_thread -lboost_system -lstdc++ -lm -I../ -I../include -I../src -o test_bmExec
