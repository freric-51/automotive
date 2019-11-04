cd latex
call make.bat

del dir*.pdf
del .pdf

del Makefile
rmdir /S /Q d0
rmdir /S /Q d1
rmdir /S /Q d2
rmdir /S /Q d3
rmdir /S /Q d4
rmdir /S /Q d5
rmdir /S /Q d6
rmdir /S /Q d7
rmdir /S /Q d8
rmdir /S /Q d9
rmdir /S /Q da
rmdir /S /Q db
rmdir /S /Q dc
rmdir /S /Q dd
rmdir /S /Q de
rmdir /S /Q df
del *.tex
del *.md5
del *.sty
del *.aux
del *.idx
del *.ilg
del *.ind
del *.log
del *.toc
del *.out
del *.bat
del *.eps

del refman.lot

rem apagar as figuras:
del c*.*
del d*.*
del l*.*
del m*.*
del t*.*

cd ..

C:\FATEC\Gerenciamento\ECU_2013\doxygen\latex\refman.pdf