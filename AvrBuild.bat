@ECHO OFF
"C:\Program Files (x86)\Atmel\AVR Tools\AvrAssembler2\avrasm2.exe" -S "C:\Programming\2019-2c-primer-proyecto-arilow\codigo\labels.tmp" -fI -W+ie -C V2E -o "C:\Programming\2019-2c-primer-proyecto-arilow\codigo\Proyecto.hex" -d "C:\Programming\2019-2c-primer-proyecto-arilow\codigo\Proyecto.obj" -e "C:\Programming\2019-2c-primer-proyecto-arilow\codigo\Proyecto.eep" -m "C:\Programming\2019-2c-primer-proyecto-arilow\codigo\Proyecto.map" "C:\Programming\2019-2c-primer-proyecto-arilow\codigo\main.asm"
