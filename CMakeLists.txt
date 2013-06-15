set (SOURCES
	ai.cpp
	edytor.cpp
	gra.cpp
	icel.cpp
	ipilka.cpp
	ipilkarzyk.cpp
	iplansza.cpp
	ipole.cpp
	kolejkaanimacji.cpp
	main.cpp
	mainwindow.cpp
	nowagradialog.cpp
	plansza.cpp
	tryb.cpp
	)

set (HEADERS
	ai.h
	edytor.h
	gra.h
	icel.h
	ipilka.h
	ipilkarzyk.h
	iplansza.h
	ipole.h
	kolejkaanimacji.h
	mainwindow.h
	nowagradialog.h
	plansza.h
	)

set (FORMS
	mainwindow.ui
	nowagradialog.ui
	)

qt4_wrap_cpp (HEADERS_MOC ${HEADERS})
qt4_wrap_ui (FORMS_HEADERS ${FORMS})

add_executable (Diabalik ${SOURCES} ${HEADERS_MOC} ${FORMS_HEADERS} ${RESOURCES_RCC})
target_link_libraries (Diabalik ${QT_LIBRARIES})
include_directories (${CMAKE_CURRENT_BINARY_DIR})
