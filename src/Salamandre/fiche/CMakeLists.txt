set(INCROOT ${PROJECT_SOURCE_DIR}/include/Salamandre/fiche)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Salamandre/fiche)
# append in local scope
list(APPEND FILES_HEADER
    ${INCROOT}/Fiche.hpp
    ${INCROOT}/FicheConfidentielle.hpp
    ${INCROOT}/FicheDonneesMedicales.hpp
    ${INCROOT}/FicheDonneesNumeriques.hpp
    ${INCROOT}/FicheEtatCivil.hpp
)
# set in parent scope
set(FILES_HEADER "${FILES_HEADER}" PARENT_SCOPE)

list(APPEND FILES_SRC
    ${SRCROOT}/Fiche.cpp
    ${SRCROOT}/FicheConfidentielle.cpp
    ${SRCROOT}/FicheDonneesMedicales.cpp
    ${SRCROOT}/FicheDonneesNumeriques.cpp
    ${SRCROOT}/FicheEtatCivil.cpp
)

# set in parent scope
set(FILES_SRC "${FILES_SRC}" PARENT_SCOPE)
