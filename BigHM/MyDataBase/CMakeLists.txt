add_library(MyDataBase_1
        conditionParser/ExprChecker.cpp conditionParser/ExprChecker.h
        dataBase/DataBase.cpp dataBase/DataBase.h
        domain/ColTable.cpp domain/ColTable.h
        domain/DataBaseType.cpp domain/DataBaseType.h
        domain/Element.cpp domain/Element.h
        table/Table.cpp table/Table.h
        utils/utils.cpp utils/utils.h
)
target_include_directories(MyDataBase_1 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})