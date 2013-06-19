set (SOURCES
	ai.cpp
	aistan.cpp
	edytor.cpp
	gra.cpp
	icel.cpp
	ipilka.cpp
	ipilkarzyk.cpp
	iplansza.cpp
	ipole.cpp
	main.cpp
	mainwindow.cpp
	nowagradialog.cpp
	plansza.cpp
	ruch.cpp
	tryb.cpp
	)

set (HEADERS
	ai.h
	aistan.h
	edytor.h
	gra.h
	icel.h
	ipilka.h
	ipilkarzyk.h
	iplansza.h
	ipole.h
	mainwindow.h
	nowagradialog.h
	plansza.h
	ruch.h
	tryb.h
	)

set (FORMS
	mainwindow.ui
	nowagradialog.ui
	)

qt4_wrap_cpp (HEADERS_MOC ${HEADERS})
qt4_wrap_ui (FORMS_HEADERS ${FORMS})

add_executable (Diaballik ${SOURCES} ${HEADERS_MOC} ${FORMS_HEADERS} ${RESOURCES_RCC})
target_link_libraries (Diaballik ${QT_LIBRARIES})
include_directories (${CMAKE_CURRENT_BINARY_DIR})


