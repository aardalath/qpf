 1147  cd '/home'
 1148  cd '/'
 1149  cd '/media'
 1150  cd '/media/jcgonzalez'
 1151  cd '/media/jcgonzalez/PAXPORT'
 1152  cd '/'
 1153  cd '/media/jcgonzalez/PAXPORT'
 1154  cd '/'
 1155  cd '/media'
 1156  cd '/'
 1157  cd '/media'
 1158  cd '/media/jcgonzalez'
 1159  cd '/media/jcgonzalez/SANDISK'
 1160  cd '/media/jcgonzalez/PAXPORT'
 1161  cd '/media/jcgonzalez/PAXPORT/Personal'
 1162  cd '/media/jcgonzalez/PAXPORT/Personal/Papeleo'
 1163  cd '/media/jcgonzalez/PAXPORT/Personal/Papeleo/Hacienda'
 1164  cd '/media/jcgonzalez/PAXPORT/Personal/Papeleo'
 1165  cd '/media/jcgonzalez/PAXPORT/Personal/Papeleo/Casas'
 1166  cd '/media/jcgonzalez/PAXPORT/Personal/Papeleo/Casas/Casa__Boadilla_CaboDeGata_8-33'
 1167  cd '/media/jcgonzalez/PAXPORT/Personal/Papeleo/Casas/Casa__Boadilla_CaboDeGata_8-33/Nueva casa'
 1168  cd '/media/jcgonzalez/PAXPORT/Personal/Papeleo/Casas/Casa__Boadilla_CaboDeGata_8-33/Nueva casa/Fisterra - Gestilar'
 1169  cd '/media/jcgonzalez/PAXPORT/Personal/Papeleo/Casas/Casa__Boadilla_CaboDeGata_8-33/Nueva casa/Fisterra - Gestilar/Libro del Edificio'
 1170  cd '/media/jcgonzalez/SANDISK'
 1171  cd '/media/jcgonzalez'
 1172  cd '/media'
 1173  cd '/'
 1174  cd '/home'
 1175  cd '/home/jcgonzalez'
 1176  cd '/home/jcgonzalez/ws'
 1177  cd '/home/jcgonzalez/ws/personal'
 1178  cd '/home/jcgonzalez/ws/personal/DPF'
 1179  cd '/home/jcgonzalez/ws/personal/DPF/code'
 1180  cd '/home/jcgonzalez/ws/personal/DPF/code/dpfdemo'
 1181  cd '/home/jcgonzalez/ws/personal/DPF/code/dpfdemo/src'
 1182  cd '/home/jcgonzalez/ws/personal/DPF/code/dpfdemo'
 1183  cd '/home/jcgonzalez/ws/personal/DPF/code'
 1184  cd '/home/jcgonzalez/ws/personal/DPF/code/dpfStimuli'
 1185  cd '/home/jcgonzalez/ws/personal/DPF/code/dpfStimuli/src'
 1186  sudo rm -rf /media/jcgonzalez/EUCL06BCK/old_201605* ; sudo rsync -abv --backup-dir=old_$(date +%Y%m%dT%H%M%S) --delete --exclude=old_* /home/jcgonzalez/ /media/jcgonzalez/EUCL06BCK/
 1187  scp eucops@eucdev02.net3.lan:/qpf-orig/data/orig/chlg6/* Documents/EUCLID/7-Data/Challenge6/
 1188  scp eucops@eucdev02.net3.lan:qpf-orig/data/orig/chlg6/* Documents/EUCLID/7-Data/Challenge6/
 1189  scp -r eucops@eucdev02.net3.lan:qpf-orig/data/orig/chlg6/* Documents/EUCLID/7-Data/Challenge6/
 1190  ls
 1191  find . -name "keys*"
 1192  cd ws/
 1193  ls
 1194  find . -name "*alert*"
 1195  less ./QPF/trunk/src/alert.h
 1196  cd QPF/trunk/src/
 1197  ./main-alert 
 1198  scp alert.cpp alert.h main-alert.cpp eucops@eucdev02.net3.lan:ws/jcgg/QPF/branches/hmicore/src/
 1199  cd
 1200  cat bin/HourlyBackup.sh 
 1201  cd Documents/EUCLID
 1202  scp eucops@eucdev02.net3.lan:QPF.all* .
 1203  tar tvzf QPF.all.tgz 
 1204  ssh -X -C eucops@eucdev02.net3.lan
 1205  ssh -X -C eucops@eucdev02.net3.lan
 1206  ssh -X -C eucops@eucdev02.net3.lan
 1207  ssh -X -C eucops@eucdev03.net3.lan
 1208  find 
 1209  find . -name "*.xsd"
 1210  ssh -X -C eucops@eucdev02.net3.lan
 1211  ssh -X -C eucops@eucdev03.net3.lan
 1212  ssh -X -C eucops@eucdev02.net3.lan
 1213  ssh -X -C eucops@eucdev.n1data.lan
 1214  ssh -X -C eucops@eucdev02.net3.lan
 1215  ssh -X -C eucops@eucdev02.net3.lan
 1216  sudo rm -rf /media/jcgonzalez/EUCL06BCK/old_201606* ; sudo rsync -abv --backup-dir=old_$(date +%Y%m%dT%H%M%S) --delete --exclude=old_* /home/jcgonzalez/ /media/jcgonzalez/EUCL06BCK/
 1217  ssh euclodeen.net3.lan -Yl user
 1218  cd ws/QDT/euclidqla/trunk/schema/
 1219  ls
 1220  svn info
 1221  scp eucops@eucdev02.net3.lan:qpf/data/test/in/EUC_VIS_*{83,93}* ~/shared/
 1222  sudo apt-get update
 1223  sudo apt-get upgrade
 1224  sudo apt-get autoremove
 1225  man shutdown
 1226  shutdown +30
 1227  cp Downloads/HIPE_14_1_0_INSTALLER.bin shared/
 1228  mv Downloads/HIPE_14_1_0_INSTALLER.bin shared/
 1229  scp eucops@eucdev02.net3.lan:qpf/run/20160713T095408/tsk/TskAge2-20160713T095812-1/out/EUC_VIS_REP_W-83114-4_20200707T144425Z.json shared/
 1230  tar tvzf shared/cots-lodeen12.tgz 
 1231  sudo rm -rf /media/jcgonzalez/EUCL06BCK/old_201606* ; sudo rsync -abv --backup-dir=old_$(date +%Y%m%dT%H%M%S) --delete --exclude=old_* /home/jcgonzalez/ /media/jcgonzalez/EUCL06BCK/
 1232  cd ws/QDT/euclidqla/trunk/
 1233  cvs update
 1234  ssh -X -C eucops@eucdev.n1data.lan
 1235  cd 
 1236  cd ws/
 1237  ls
 1238  rm EAS.tgz 
 1239  scp eucops@eucdev.n1data.lan:ws/jcgg/EAS_CVSLastUpdate20160719.tgz
 1240  scp eucops@eucdev.n1data.lan:ws/jcgg/EAS_CVSLastUpdate20160719.tgz .
 1241  ls -l EAS/
 1242  tar xvzf EAS_CVSLastUpdate20160719.tgz 
 1243  ll
 1244  cp EAS_CVSLastUpdate20160719.tgz ~/shared/
 1245  ssh -X -C eucops@eucdev02.net3.lan
 1246  ls ~/ws/QDT/euclidqla/trunk/euclidqla/sessionlog/
 1247  python ~/ws/QDT/euclidqla/trunk/euclidqla/sessionlog/sessionlog.py 
 1248  cat /tmp/test_logger.log 
 1249  ssh -X -C eucops@eucdev02.net3.lan
 1250  grep herschel *
 1251  grep ggw *
 1252  ssh -X -C jcgonzalez@ggw3-herschel
 1253  ls
 1254  grep her hist*
 1255  grep her *
 1256  ssh -X -C ggw-herschel-d3
 1257  echo "ssh -X -C ggw-herschel-d3" > bin/herschel3.sh
 1258  chmod 755 bin/herschel3.sh
 1259  echo "ssh -X -C ggw-herschel-d2" > bin/herschel2.sh
 1260  chmod 755 bin/herschel2.sh
 1261  herschel3.sh 
 1262  man tr
 1263  ps x
 1264  kill -9 8044
 1265  hexdump -e '80/1 "%_p" "\n"' Documents/EUCLID_data/Images/EUC_VIS.fits 
 1266  hexdump -e '"%_ad\t" 80/1 "%_p" "\n"'  Documents/EUCLID_data/Images/EUC_VIS.fits 
 1267  hexdump -e '"%_ad\t" 80/1 "%_p" "\n"'  Documents/EUCLID_data/Images/EUC_VIS.fits |less
 1268  hexdump -e '80/1 "%_p" "\n"' Documents/EUCLID_data/Images/EUC_VIS.fits |less
 1269  tr -d '^[:print:]' Documents/EUCLID_data/Images/EUC_VIS.fits  |less    
 1270  cat Documents/EUCLID_data/Images/EUC_VIS.fits | tr -d '^[:print:]' |less    
 1271  cat Documents/EUCLID_data/Images/EUC_VIS.fits | tr -d -s -c '[:print:]' |less    
 1272  cat Documents/EUCLID_data/Images/EUC_VIS.fits | tr -d -c '[:print:]' |less    
 1273  cat Documents/EUCLID_data/Images/EUC_VIS.fits | tr -d '^[:print:]' |less     
 1274  cat Documents/EUCLID_data/Images/EUC_VIS.fits | tr -d '^[:print:]' |fmt -w80 |less     
 1275  cat Documents/EUCLID_data/Images/EUC_VIS.fits | tr -d -c '[:print:]' |fmt -w80 |less     
 1276  cat Documents/EUCLID_data/Images/EUC_VIS.fits | tr -d -c '[:print:]' |fmt -w76 |less     
 1277  cat Documents/EUCLID_data/Images/EUC_VIS.fits | tr -d -c '[:print:]' |fmt -w40 |less     
 1278  cat Documents/EUCLID_data/Images/EUC_VIS.fits | tr -c '[:print:]' ' ' |fmt -w40 |less     
 1279  cat Documents/EUCLID_data/Images/EUC_VIS.fits | tr -c '[:print:]' ' ' |fmt -w80 |less     
 1280  cat Documents/EUCLID_data/Images/EUC_VIS.fits | tr -c '[:print:]' ' ' |fmt -w82 |less     
 1281  cat Documents/EUCLID_data/Images/EUC_VIS.fits | tr -c '[:print:]' ' ' |fmt -w88 |less     
 1282  cat Documents/EUCLID_data/Images/EUC_VIS.fits | tr -c '[:print:]' ' ' |fmt -w85 |less     
 1283  od -c Documents/EUCLID_data/Images/EUC_VIS.fits
 1284  od -c Documents/EUCLID_data/Images/EUC_VIS.fits|less
 1285  man od
 1286  od -c Documents/EUCLID_data/Images/EUC_VIS.fits -w64 |less
 1287  od -c Documents/EUCLID_data/Images/EUC_VIS.fits -w60 |less
 1288  od -c Documents/EUCLID_data/Images/EUC_VIS.fits -w48 |less
 1289  od -c Documents/EUCLID_data/Images/EUC_VIS.fits -w52 |less
 1290  od -c Documents/EUCLID_data/Images/EUC_VIS.fits -w70 |less
 1291  od -c Documents/EUCLID_data/Images/EUC_VIS.fits -w80 |less
 1292  man fread
 1293  man strcmp
 1294  history
 1295  ./tr
 1296  ls -ltr
 1297  mv bin/trytogetfitsheader* .
 1298  history
 1299  ./trytogetfitsheader Documents/EUCLID_data/Images/EUC_VIS.fits|less
 1300  less Documents/EUCLID_data/Images/EUC_VIS.fits
 1301  ./trytogetfitsheader Documents/EUCLID_data/Images/EUC_VIS.fits|less
 1302  sudo rm -rf /media/jcgonzalez/EUCL06BCK/old_201606* ; sudo rsync -abv --backup-dir=old_$(date +%Y%m%dT%H%M%S) --delete --exclude=old_* /home/jcgonzalez/ /media/jcgonzalez/EUCL06BCK/
 1303  ls -l
 1304  mc
 1305  sudo apt-get install vlc
 1306  locate avidemux
 1307  /usr/bin/avidemux
 1308  ls -l/usr/bin/avidemux
 1309  ls -l /usr/bin/avidemux
 1310  ls -l /etc/alternatives/avidemux 
 1311  ls -l /usr/bin/avidemux2_gtk
 1312  sudo apt-get update
 1313  sudo apt-get install avidemux
 1314  sudo apt-get install mencoder
 1315  mencoder Desktop/d7840284-8e6c-402f-9ebf-018c39da41f4.mp4  -oac lavc -ovc x264 -vf rotate=1 -o alba-robots.mp4
 1316  mv alba-robots.mp4 Desktop/
 1317  mencoder --help
 1318  man mencoder 
 1319  mencoder Desktop/d7840284-8e6c-402f-9ebf-018c39da41f4.mp4  -oac lavc -ovc x264 -vf rotate=0 -o alba-robots.mp4
 1320  mv alba-robots.mp4 ~/Desktop/
 1321  mplayer Desktop/alba-robots.mp4 
 1322  cd ws/QDT/
 1323  ls
 1324  cd euclidqla/
 1325  ls
 1326  cd trunk/
 1327  ls -ltr
 1328  svn info
 1329  locate fits
 1330  locate fits|grep map
 1331  locate fits|grep vis
 1332  shutdown --help
 1333  an shutdown 
 1334  man shutdown 
 1335  shutdown -P +15
 1336  mv shared/qla-maps/c2
 1337  mkdir shared/qla-maps/c2
 1338  mv shared/qla-maps/* shared/qla-maps/c2/
 1339  mkdir shared/qla-maps/c1
 1340  scp eucdev02.net3.lan:/home/eucops/qpf/bin/QDT/tools/*.fits shared/qla-maps/c1/
 1341  scp eucops@eucdev02.net3.lan:/home/eucops/qpf/bin/QDT/tools/*.fits shared/qla-maps/c1/
 1342  cd shared/qla-maps/
 1343  ls
 1344  ll
 1345  find .
 1346  cmp ./c2/bad_pixel_mask.fits ./c1/bad_pixel_mask.fits
 1347  cmp ./c2/vis_sat_map.fits ./c1/vis_sat_map.fits
 1348  rm -rf ./c2/bad_pixel_mask.fits
 1349  cmp ./c1/bad_pixel_mask.fits ./c2/bpm2.fits
 1350  rm -rf ./c2/bpm2.fits
 1351  find .
 1352  mplayer ~/Desktop/alba-robots.mp4 
 1353  rm -rf shared/Images/
 1354  mkdir shared/qla-tools
 1355  mv shared/qla-tools shared/qla-maps
 1356  scp user@eucldeen.net3.lan:qla/tools/*.fits shared/qla-maps/
 1357  scp user@euclodeen.net3.lan:qla/tools/*.fits shared/qla-maps/
 1358  scp -r user@euclodeen.net3.lan:qla/TestFiles shared/qla-testimages
 1359  cd ws/QDT/
 1360  svn update
 1361  ll
 1362  cd euclidqla/
 1363  ls
 1364  rm -rf ~/shared/QDT-euclidqla ; cp -r trunk/euclidqla ~/shared/QDT-euclidqla
 1365  ls shared/
 1366  cat bin/lodeen.sh 
 1367  scp shared/cots-lodeen12.tgz user@euclodeen.net3.lan:
 1368  scp .ssh/id_rsa.pub user@euclodeen.net3.lan:.ssh/id_rsa.jcgonzalez-at-eucl06.pub
 1369  ll .ssh
 1370  cd .ssh/
 1371  scp user@euclodeen.net3.lan:.ssh/id_rsa.pub .ssh/id_rsa.user-at-euclodeen.pub
 1372  scp user@euclodeen.net3.lan:.ssh/id_rsa.pub id_rsa.user-at-euclodeen.pub
 1373  cat id_rsa.user-at-euclodeen.pub >> known_hosts 
 1374  cat id_rsa.user-at-euclodeen.pub >> authorized_keys
 1375  cd
 1376  scp shared/cots-lodeen12.tgz user@euclodeen.net3.lan:/home/user/
 1377  lodeen.sh 
 1378  ssh -X -C eucops@eucdev02.net3.lan
 1379  ls bin/
 1380  lodeen.sh 
 1381  locate .iso
 1382  libvirtd -d
 1383  libvirtd 
 1384  man libvirtd
 1385  libvirtd -d
 1386  libvirtd 
 1387  libvirtd 
 1388  ps x
 1389  libvirtd -d
 1390  libvirtd 
 1391  chmod 644 ~/shared/*.out
 1392  okular ~/output.pdf 
 1393  sudo rm -rf /media/jcgonzalez/EUCL06BCK/old_201606* ; sudo rsync -abv --backup-dir=old_$(date +%Y%m%dT%H%M%S) --delete --exclude=old_* /home/jcgonzalez/ /media/jcgonzalez/EUCL06BCK/
 1394  sudo rm -rf /media/jcgonzalez/EUCL06BCK/old_20160706T090855 /media/jcgonzalez/EUCL06BCK/old_20160715T163714 ; sudo rsync -abv --backup-dir=old_$(date +%Y%m%dT%H%M%S) --delete --exclude=old_* /home/jcgonzalez/ /media/jcgonzalez/EUCL06BCK/
 1395  ls
 1396  cp -r Personal/Research shared/
 1397  sudo chown -r jcgonzalez.jcgonzalez Personal/Research
 1398  sudo chown -R jcgonzalez.jcgonzalez Personal/Research
 1399  sudo cp -r Personal/Research shared/
 1400  +
 1401  sudo chown -R jcgonzalez.jcgonzalez shared/Research
 1402  ps x
 1403  kill -9 
 1404  kill -9 4987
 1405  cd '/home/jcgonzalez/ws/personal/DPF/code/dpfStimuli/src'
 1406  cd '/home/jcgonzalez/ws/personal/DPF/code/dpfStimuli'
 1407  cd '/home/jcgonzalez/ws/personal/DPF/code'
 1408  cd '/home/jcgonzalez/ws/personal/DPF'
 1409  cd '/home/jcgonzalez/ws/personal'
 1410  cd '/home/jcgonzalez/ws'
 1411  cd '/home/jcgonzalez'
 1412  cd '/media/jcgonzalez'
 1413  cd '/media/jcgonzalez/JCUltra1TB'
 1414  cd '/media/jcgonzalez/JCUltra1TB/PASSPORT'
 1415  cd '/media/jcgonzalez/JCUltra1TB/PASSPORT/Personal'
 1416  cd '/media/jcgonzalez/JCUltra1TB/PASSPORT/Personal/Projects'
 1417  cd '/media/jcgonzalez/JCUltra1TB/PASSPORT/Personal'
 1418  cd '/media/jcgonzalez/JCUltra1TB/PASSPORT/Personal/Documentos_Propios'
 1419  cd '/media/jcgonzalez/JCUltra1TB/PASSPORT/Personal/Documentos_Propios/JC'
 1420  cd '/media/jcgonzalez/JCUltra1TB/PASSPORT/Personal/Documentos_Propios/JC/Literatura'
 1421  cd '/media/jcgonzalez/JCUltra1TB/PASSPORT/Personal/Documentos_Propios/JC/Literatura/Libros'
 1422  cd '/media/jcgonzalez/JCUltra1TB/PASSPORT/Personal/Documentos_Propios/JC/Literatura/Libros/NoctisLabyrinthus'
 1423  cd '/media/jcgonzalez/JCUltra1TB/PASSPORT/Personal/Documentos_Propios/JC/Literatura/Libros/NoctisLabyrinthus/5-maqueta'
 1424  cd '/home/jcgonzalez'
 1425  cd '/media/jcgonzalez/JCPEN'
 1426  cd '/media/jcgonzalez/JCPEN/Transubstanciacion'
 1427  cd '/media/jcgonzalez/JCUltra1TB/PASSPORT/Personal/Documentos_Propios/JC/Literatura/Libros/NoctisLabyrinthus/5-maqueta'
 1428  cd '/media/jcgonzalez/JCPEN/Transubstanciacion'
 1429  cd '/media/jcgonzalez/JCPEN/Transubstanciacion/5-maqueta'
 1430  cd '/media/jcgonzalez/JCUltra1TB/PASSPORT/Personal/Documentos_Propios/JC/Literatura/Libros/NoctisLabyrinthus/5-maqueta'
 1431  cd '/media/jcgonzalez/JCUltra1TB/PASSPORT/Personal/Documentos_Propios/JC/Literatura/Libros/NoctisLabyrinthus/5-maqueta/cover'
 1432  cd '/media/jcgonzalez/JCUltra1TB/PASSPORT/Personal/Documentos_Propios/JC/Literatura/Libros/NoctisLabyrinthus/5-maqueta/cover/book'
 1433  cd '/media/jcgonzalez/JCUltra1TB/PASSPORT/Personal/Documentos_Propios/JC/Literatura/Libros/NoctisLabyrinthus/5-maqueta/cover'
 1434  cd '/media/jcgonzalez/JCUltra1TB/PASSPORT/Personal/Documentos_Propios/JC/Literatura/Libros/NoctisLabyrinthus/5-maqueta'
 1435  cd '/media/jcgonzalez/JCPEN/Transubstanciacion/5-maqueta'
 1436  ls -l
 1437  cal 2161
 1438  man cal
 1439  cal -M 2161
 1440  cal -M -y 2161
 1441  man cal
 1442  ncal -M -y 2161
 1443  find . -name "*.mp*" -ls
 1444  find . -name "*.aac" -ls
 1445  find . -name "*.avi" -ls
 1446  find . -name "*ova"
 1447  find . -size +200M
 1448  find . -size +200M -ls
 1449  cd ./bak/HourlyBackup/16/home/jcgonzalez/Documents/EUCLID/7-Data/Challenge6/Images/
 1450  ls -l
 1451  mkdir nobackup
 1452  mv *.fits nobackup/
 1453  ll
 1454  cd ..
 1455  cd -
 1456  cd
 1457  mplayer ./Desktop/Summer/Fotos\ m\303\263vil\ 20160807/VID_20160729_161945809.mp4
 1458  mplayer ./Desktop/Summer/Fotos\ móvil\ 20160807/VID_2016072*
 1459  find . -size +200M -ls
 1460  rm -rf ./bak/HourlyBackup/*/home/jcgonzalez/Documents/EUCLID/7-Data/Challenge6/Images/*.fits
 1461  find . -size +200M -ls
 1462  rm -rf ./bak/HourlyBackup/16/home/jcgonzalez/Documents/EUCLID/7-Data/Challenge6/Images/nobackup/*.fits
 1463  bzip2 ./Personal/Research/Astrophysics/Astrostatistics/www2.astro.psu.edu/edf/edf/GLM_28800-0050_mosaic_I4.fits
 1464  bg
 1465  cmp ./shared/qla-maps/vis_sat_map-2.fits ./shared/qla-maps/vis_sat_map.fits
 1466  du -ks .
 1467  df -k
 1468  rm -rf ./ws/personal/tests/ARCH_SeveralPkgs-20150312.tar.gz ./ws/personal/tests/ARCH_DPF-DEMO_Old_Eclipse_GUI-20150309.tar.gz
 1469  ll ./Personal/Research/Astrophysics/Astrostatistics/www2.astro.psu.edu/edf/edf/GLM_28800-0050_mosaic_I4.fits
 1470  ll ./Personal/Research/Astrophysics/Astrostatistics/www2.astro.psu.edu/edf/edf/GLM_28800-0050_mosaic_I4.fits.bz2 
 1471  bzip2 ./Personal/Research/Astrophysics/Astrostatistics/www2.astro.psu.edu/edf/edf/*.fits &
 1472  htop 
 1473  ps x
 1474  ll ./Personal/Research/Astrophysics/Astrostatistics/www2.astro.psu.edu/edf/edf/*.fits.*
 1475  df -k
 1476  ls bin/restartVBoxDrv.sh 
 1477  cat bin/restartVBoxDrv.sh 
 1478  sudo /etc/init.d/vboxdrv setup
 1479  rapidsvn ^
 1480  find . -name "*uclid*png"
 1481  find . -name "*uclid*jp*"
 1482  eog ./Pictures/Euclid_logo_node_full_image_2.jpg
 1483  eog Documents/EUCLID/2-Images/Euclid_logo.jpg 
 1484  cd '/home/jcgonzalez'
 1485  cd ws/QPF/trunk/
 1486  svn info 
 1487  cd
 1488  find . -name "*.[pj][np]g"
 1489  find . -name "*.[pj][np]g"|grep -v HourlyBackup
 1490  find . -name "*.[pj][np]g"|grep -v HourlyBackup > imgs
 1491  find . -name "*.[pj][np]g"|grep -v HourlyBackup |grep -v .cache > imgs
 1492  sudo cp /etc/sudoers sudoers
 1493  cd tmp/n1
 1494  ls
 1495  cd N1-master/
 1496  ls
 1497  less README.md 
 1498  ls
 1499  cd build/
 1500  ls
 1501  cd ..
 1502  less N1.sh 
 1503  cd
 1504  cd ws/QDT/
 1505  ls
 1506  cd ..
 1507  ps x
 1508  kill -9 16445
 1509  cd QDT/
 1510  ls
 1511  svn info
 1512  svn mv  https://euclid.esac.esa.int/svn/ESA/SOC/SOC-3-DEV/SOC-3-07-QLook/QLDT  https://euclid.esac.esa.int/svn/ESA/SOC/SOC-3-DEV/SOC-3-07-QLook/QDT
 1513  svn info
 1514  svn update
 1515  ls
 1516  cd ..
 1517  ls
 1518  rm QDT QDT-orig
 1519  mv QDT QDT-orig
 1520  svn checkout  https://euclid.esac.esa.int/svn/ESA/SOC/SOC-3-DEV/SOC-3-07-QLook/QDT
 1521  cd
 1522  find . -name "QL*"
 1523  find . -name "QL*"|grep -v HourlyBackup
 1524  find . -name "consolid*"
 1525  cd ./ws/QDT/euclidqla/branches/consolidated_report
 1526  ls -l
 1527  find . -ls
 1528  find . -name "*.log"
 1529  cd ..
 1530  ls
 1531  cd ..
 1532  ls
 1533  cd ..
 1534  ls
 1535  cd ..
 1536  ls
 1537  cd ..
 1538  find . -name "*.log"
 1539  find . -name "*.log"|xargs ls -ltr
 1540  ssh -X -C eucops@eucdev02.net3.lan
 1541  cd ws/QDT
 1542  ls
 1543  cd ..
 1544  ls
 1545  mkdir EAS/
 1546  ls
 1547  cd EAS/
 1548  ls
 1549  mkdir new
 1550  mkdir CVS
 1551  mkdir SVN
 1552  mv new CVS/
 1553  cd CVS/
 1554  rmdir new
 1555  cvs co cd ..
 1556  cd ../euclid/
 1557  ls
 1558  cvs info
 1559  cat CVS/Root 
 1560  cat CVS/Repository 
 1561  cd CVS/
 1562  cvs co jcgonzalez@cvs.astro-wise.org:/cvsroot
 1563  cvs co -d jcgonzalez@cvs.astro-wise.org:/cvsroot
 1564  cvs -d jcgonzalez@cvs.astro-wise.org:/cvsroot co 
 1565  cvs -d jcgonzalez@cvs.astro-wise.org:/cvsroot co  euclid
 1566  ssh -X -C eucops@eucdev02.n1data.lan
 1567  ssh -X -C eucops@eucdev.n1data.lan
 1568  cd ..
 1569  mkdir SVN
 1570  cd SVN/
 1571  svn co http://euclid.esac.esa.int/svn/EC/SGS/ST/4-2-07-EAS/eascode/
 1572  ssh -X -C eucops@eucdev.n1data.lan
 1573  cd ws/EAS/
 1574  ls
 1575  cd SVN/
 1576  ls
 1577  svn checkout http://euclid.esac.esa.int/svn/EC/SGS/ST/4-2-07-EAS/eascode/
 1578  cd eascode/
 1579  ls
 1580  svn log -v --xml
 1581  svn log -v --xml|xmllint 
 1582  svn log -v --xml|xmllint --format
 1583  svn log -v --xml|xmllint --format -
 1584  ls
 1585  cd '/home/jcgonzalez'
 1586  ll
 1587  mv Downloads/pymetrics-0.8.1.tar.gz ~/shared/
 1588  ls revisi*
 1589  find . -name "revisi*"
 1590  find ws -name "revisi*"
 1591  cd
 1592  cd ws/QPF/trunk/
 1593  ~/bin/sonar-scanner-2.7/bin/sonar-scanner 
 1594  ll
 1595  ls -ltr
 1596  ll
 1597  ls -ltr
 1598  less defaults.cmake 
 1599  find . -name "*ake*"
 1600  mv build build-qmake
 1601  mkdir build-cmake
 1602  cd build-cmake/
 1603  ccmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ../
 1604  cd
 1605  cd ws/
 1606  ls
 1607  cd personal/
 1608  tar tvzf ARCH_DPF-original-20150127.tar.gz |less
 1609  mkdir ARCH_DPF
 1610  cd ARCH_DPF/
 1611  tar xvzf ../ARCH_DPF-original-20150127.tar.gz 
 1612  cd 
 1613  cd ws/QPF/trunk/
 1614  svn update
 1615  ll
 1616  cd 
 1617  cd bin/sonarqube-6.0/
 1618  ls
 1619  cd bin/
 1620  ls
 1621  cd linux-x86-64/
 1622  ls
 1623  cat sonar.sh 
 1624  ./sonar.sh status
 1625  ./sonar.sh start
 1626  ./sonar.sh status
 1627  ./sonar.sh console
 1628  ./sonar.sh stop
 1629  cd ../../
 1630  cd ..
 1631  ls
 1632  cd SourceMeter-8.1.0-x64-linux/
 1633  ls
 1634  cd CPP/
 1635  ls
 1636  ./SourceMeterCPP 
 1637  ./SourceMeterCPP ~/ws/QPF/trunk/src/*.cpp-resultsDir
 1638  ./SourceMeterCPP -resultsDir=./QPF:trunk
 1639  ./SourceMeterCPP -resultsDir=./QPF-trunk -projectName=QPF-trunk
 1640  ./SourceMeterCPP -resultsDir=./QPF-trunk -projectName=QPF-trunk -buildScript=ls
 1641  ./SourceMeterCPP -resultsDir=./QPF-trunk -projectName=QPF-trunk -buildScript=/bin/ls
 1642  ./SourceMeterCPP -resultsDir=./QPF-trunk -projectName=QPF-trunk -buildScript=/bin/ls ~/ws/QPF/trunk/src/*.cpp
 1643  ll
 1644  ls -ltr
 1645  find QPF-trunk/QPF-trunk
 1646  cd ..
 1647  ls
 1648  cd ..
 1649  ls
 1650  cd FlintPlusPlus-master/
 1651  ls
 1652  cd bin/
 1653  ls
 1654  cd x64/
 1655  ls
 1656  cd ../deb64/
 1657  ll
 1658  less flint++ 
 1659  chmod 777 flint++ 
 1660  ./flint++ 
 1661  ./flint++ ~/ws/QPF/trunk
 1662  ./flint++ -r ~/ws/QPF/trunk
 1663  ./flint++ -r -l2 -j ~/ws/QPF/trunk
 1664  ./flint++ -l2 -j ~/ws/QPF/trunk/{libcomm,infix,src,qpfgui}/*.cpp
 1665  ./flint++ -l2  ~/ws/QPF/trunk/{libcomm,infix,src,qpfgui}/*.cpp
 1666  ll
 1667  cd ..
 1668  ls -ltr
 1669  cd conqat-2015.2/
 1670  ls
 1671  ./eclipse 
 1672  cd ../
 1673  cd oclint-0.10.3/
 1674  ls
 1675  cd bin/
 1676  ls
 1677  export OCLINT_HOME=$(pwd)
 1678  export PATH=$PATH:$OCLINT_HOME
 1679  cd ..
 1680  export OCLINT_HOME=$(pwd)
 1681  oclint
 1682  oclint -hlep
 1683  oclint -help
 1684  oclint -enable-clang-static-analyzer -p ~/ws/QPF/trunk/build
 1685  oclint -enable-clang-static-analyzer -p ~/ws/QPF/trunk/build ~/ws/QPF/trunk/src/*
 1686  ll
 1687  ruby
 1688  cd ..
 1689  cd ws/
 1690  ls
 1691  cd QPF/trunk/
 1692  ls
 1693  cd
 1694  cd bin
 1695  chmod 755 qmake2cmake.*
 1696  qmake2cmake.sh ~/ws/QPF/trunk
 1697  cd Downloads/
 1698  ls
 1699  ls -ltr
 1700  tar xvzf cyclo-2.0.tar.gz 
 1701  tar xvf cyclo-2.0.tar.gz 
 1702  cd cyclo-2.0/
 1703  ls
 1704  ll
 1705  ls -ltr ..
 1706  ls -ltr ../..
 1707  patch -p0 cyclo-patch-for-gcc.patch
 1708  patch < cyclo-patch-for-gcc.patch
 1709  patch -p0 ~/cyclo-patch-for-gcc.patch
 1710  patch < ~/cyclo-patch-for-gcc.patch
 1711  ls -ltr
 1712  make
 1713  man getopt
 1714  man 2 getopt
 1715  man 3 getopt
 1716  cyclo 
 1717  cyclo -l -t -c -f -F
 1718  cat main.C |cyclo -l -t -c -f -F
 1719  cat main.C |cyclo -l -t
 1720  cat main.C |cyclo -l -t -c
 1721  cat main.C |cyclo -l -t -f
 1722  cat main.C |cyclo -l -t -F
 1723  cat main.C |cyclo -l
 1724  cat main.C |cyclo -t
 1725  cat main.C |mcstrip | cyclo -t
 1726  cat main.C |mcstrip | cyclo -f
 1727  cat main.C |mcstrip | cyclo -f -F
 1728  cat main.C |mcstrip | cyclo -c
 1729  for i in src libcomm infix str qpfgui ; do echo "##### $i #############################"; for f in $(find /home/jcgonzalez/ws/QPF/trunk/$i \( -name "*.cpp" -o -name "*.h" \) -print ) ; do echo "#==== $f "; cat $f | mcstrip | ./cyclo -c ; done; done
 1730  for i in src libcomm infix str qpfgui ; do echo "##### $i #############################"; for f in $(find /home/jcgonzalez/ws/QPF/trunk/$i \( -name "*.cpp" \) -print ) ; do echo "#==== $f "; cat $f | mcstrip | ./cyclo -c ; done; done
 1731  for i in src libcomm infix str qpfgui ; do echo "##### $i #############################"; for f in $(find /home/jcgonzalez/ws/QPF/trunk/$i \( -name "*.cpp" \) -print ) ; do echo "#==== $f "; cat $f | mcstrip | ./cyclo -c ; done; done > QPF.trunk.cyclo
 1732  for i in src libcomm infix str qpfgui ; do echo "##### $i #############################"; for f in $(find /home/jcgonzalez/ws/QPF/trunk/$i \( -name "*.cpp" \) -print ) ; do echo "#==== $f "; cat $f | mcstrip | ./cyclo -c ; done; done > QPF.trunk.cyclo 2>&1 
 1733  for i in src libcomm infix str qpfgui ; do echo "##### $i #############################"; for f in $(find /home/jcgonzalez/ws/QPF/trunk/$i \( -name "*.cpp" \) -print ) ; do echo "#==== $f "; cat $f | mcstrip | ./cyclo -c |awk -v f=$f '{print f":"$0;}' ; done; done > QPF.trunk.cyclo 2>&1 
 1734  for i in src libcomm infix str qpfgui ; do for f in $(find /home/jcgonzalez/ws/QPF/trunk/$i \( -name "*.cpp" \) -print ) ; do cat $f | mcstrip | ./cyclo -c |awk -v f=$f '{print f":"$0;}' ; done; done |grep -v "Total complexity" > QPF.trunk.cyclo 2>&1 
 1735  for i in src libcomm infix str qpfgui ; do for f in $(find /home/jcgonzalez/ws/QPF/trunk/$i \( -name "*.cpp" \) -print ) ; do cat $f | mcstrip | ./cyclo -c |awk -v f=$f '{print f":"$0;}' ; done; done 2>&1 |grep -v "Total complexity" | sort -n -k2 > QPF.trunk.cyclo
 1736  for i in src libcomm infix str qpfgui ; do for f in $(find /home/jcgonzalez/ws/QPF/trunk/$i \( -name "*.cpp" \) -print ) ; do cat $f | mcstrip | ./cyclo -c |awk -v f=$f '{print f":"$0;}' ; done; done 2>&1 |grep -v "Total complexity" > QPF.trunk.cyclo
 1737  for i in src libcomm infix str qpfgui ; do for f in $(find /home/jcgonzalez/ws/QPF/trunk/$i \( -name "*.cpp" \) -print ) ; do echo $f ; cat $f | mcstrip | ./cyclo -c |awk -v f=$f '{print f":"$0;}' ; done; done 2>&1 |grep -v "Total complexity" > QPF.trunk.cyclo
 1738  cd ../../bin/q2c-master/
 1739  ls
 1740  cd q2c/
 1741  qmake q2c.pro 
 1742  make
 1743  ll
 1744  cd ..
 1745  q2c ~/ws/QPF/trunk/
 1746  q2c/q2c ~/ws/QPF/trunk/
 1747  q2c/q2c -i ~/ws/QPF/trunk/QPF.pro -o ~/ws/QPF/trunk/QPF.cmake
 1748  cd ws/QPF/trunk/QPF.cmake
 1749  cd /home/jcgonzalez/ws/QPF/trunk
 1750  ~/bin/q2c-master/q2c/q2c 
 1751  cat QPF.pro
 1752  ssh -X -C eucops@eucdev.n1data.lan
 1753  cd .nylas/
 1754  ls
 1755  mv packages themes
 1756  n1
 1757  mkdir packages
 1758  mv themes styles
 1759  ls styles/
 1760  less config.json 
 1761  ls dev
 1762  mv styles dev/
 1763  sudo rm -rf /media/jcgonzalez/EUCL06BCK/old_2016072* ; sudo rsync -abv --backup-dir=old_$(date +%Y%m%dT%H%M%S) --delete --exclude=old_* /home/jcgonzalez/ /media/jcgonzalez/EUCL06BCK/
 1764  d
 1765  cd
 1766  sudo rsync -abv --backup-dir=old_$(date +%Y%m%dT%H%M%S) --delete --exclude=old_* /media/jcgonzalez/PAXPORT/ /media/jcgonzalez/JCUltra1TB/PASSPORT/; sudo rsync -abv --backup-dir=old_$(date +%Y%m%dT%H%M%S) --delete --exclude=old_* /media/jcgonzalez/SANDISK/ /media/jcgonzalez/JCUltra1TB/SANDISK/
 1767  ssh -X -C eucops@eucdev02.net3.lan
 1768  ls bin
 1769  sudo apt-get autoremove
 1770  df -k
 1771  cd /tmp
 1772  ls
 1773  ll
 1774  rm -rf .com.google.Chrome.ou3BgD/Singleton*
 1775  df -k
 1776  ls -l
 1777  cd ws/QPF/trunk/
 1778  ls -ltr
 1779  cd qpfgui
 1780  ll
 1781  ps x
 1782  df ' k
 1783  du -ks .
 1784  du -ks *
 1785  du -ks *|sort -n
 1786  cd bak/
 1787  ls
 1788  cd HourlyBackup/
 1789  ls
 1790  ls *
 1791  find 09
 1792  cd ..
 1793  cd bin/
 1794  less HourlyBackup.sh 
 1795  ll
 1796  cd ..
 1797  du -ks *
 1798  du -ks * |sort -n
 1799  cd shared/
 1800  ll
 1801  ls -s
 1802  ls -s|sort -n
 1803  rm -rf QPF_and_LibComm-20160* cots-lodeen12.tgz JSONBuddyInstaller.exe 
 1804  df -k /
 1805  df -ks /*
 1806  df -k /*
 1807  df -k /
 1808  df -k 
 1809  ps x
 1810  ps x|grep nylas
 1811  ps x|grep nylas |awk '{print $1;}'
 1812  kill -9 $(ps x|grep nylas |awk '{print $1;}')
 1813  ps x
 1814  cd ..
 1815  ls
 1816  ll
 1817  ls -l /
 1818  cd /tmp
 1819  ll
 1820  du -ks *
 1821  ls -l /var/tmp/
 1822  du -ks /*
 1823  cd /qpf
 1824  du -ks *
 1825  cd data/
 1826  du -ks *
 1827  cd test_run-challenge6
 1828  ll
 1829  cd in
 1830  ll
 1831  cd ..
 1832  ll
 1833  cd ..
 1834  ll
 1835  cd test_run
 1836  cd ..
 1837  ls -l
 1838  ls -ld
 1839  ls -ld *
 1840  find test_run
 1841  rm -rf test_run
 1842  ll
 1843  cd inbox/
 1844  ll
 1845  cd ..
 1846  cd folder_with_products/
 1847  ll
 1848  rm *
 1849  cd ..
 1850  ll
 1851  du -ks *
 1852  rm -rf test_run-mini
 1853  du -ks *
 1854  rm -rf test_run-challenge6-mini
 1855  df -k
 1856  ll
 1857  find .
 1858  find  . -ls
 1859  rm test_run-challenge6/in/*.log
 1860  rm -rf test_run-challenge6
 1861  ll
 1862  find .
 1863  rm /rf folder_with_products/
 1864  rm -rf folder_with_products/
 1865  cd ..
 1866  find .
 1867  cd ..
 1868  find .
 1869  du -ks *
 1870  cd var/
 1871  du -ks *
 1872  cd tmp/
 1873  ls
 1874  ll
 1875  cd ..
 1876  ll
 1877  cd ..
 1878  ll
 1879  cd opt/
 1880  ll
 1881  du -ks *
 1882  rm -rf android-sdk-linux/
 1883  sudo rm -rf android-sdk-linux/
 1884  ll
 1885  cd
 1886  ll
 1887  less imgs 
 1888  rm imgs 
 1889  ls -s *
 1890  ls -ds *
 1891  eog Pictures/WPD2AMEM.jpg 
 1892  eog Pictures/1151167869010.jpg 
 1893  df -k
 1894  cd ws/QPF/trunk/build
 1895  ccmake ..
 1896  cmake --help-policy CMP0046
 1897  cmake_policy CMP0046 NEW
 1898  ccmake ..
 1899  ssh -X -C -p 2020 eucops@eucdev03.net3.lan
 1900  ssh -X -C eucops@eucl03.net3.lan
 1901  ssh -Y -p 2020  -C eucops@eucl03.net3.lan
 1902  locate curl
 1903  locate libcurl
 1904  cd ws/QPF/trunk/
 1905  find . -name \( "CMake*" -o -name "*.cmake" \) -print
 1906  find . \( -name "CMake*" -o -name "*.cmake" \) -print
 1907  rm -rf qpfgui/build
 1908  find . \( -name "CMake*" -o -name "*.cmake" \) -print
 1909  rm -rf build
 1910  find . \( -name "CMake*" -o -name "*.cmake" \) -print
 1911  rm -rf build-cmake/
 1912  find . \( -name "CMake*" -o -name "*.cmake" \) -print
 1913  find . \( -name "CMake*" -o -name "*.cmake" \) -print|xargs tar cvzf cmakefiles.tgz 
 1914  tar tvzf cmakefiles.tgz 
 1915  scp cmakefiles.tgz eucops@eucdev02.net3.lan:ws/jcgg/
 1916  ssh eucops@eucl03.net3.lan -Y eucops -p 2020
 1917  ssh user@eucl03.net3.lan -Y eucops -p 2020
 1918  ssh -Y -p 2020 eucops@eucl03.net3.lan
 1919  ls -l bin/emacs
 1920  ls -l bin/emacs*
 1921  bin/emacs --debug-init
 1922  cd .emacs.
 1923  cd .emacs.d
 1924  ls -l
 1925  svn info
 1926  git info
 1927  git update
 1928  git fetch
 1929  cd
 1930  bin/emacs --debug-init
 1931  locate utop.el
 1932  rm -rf /home/jcgonzalez/.emacs.d/elpa/utop-20151105.247/utop.elc /home/jcgonzalez/.emacs.d-prev/elpa/utop-20150112.156/utop.elc /home/jcgonzalez/.emacs.d.PRELUDE-orig/elpa/utop-20151105.247/utop.elc /home/jcgonzalez/ws/.emacs.d/elpa/utop-20150112.156/utop.elc
 1933  bin/emacs --debug-init
 1934  df -k
 1935  sudo apt update
 1936  bin/emacs -nw
 1937  bin/emacs --debug-init
 1938  bin/emacs
 1939  bin/emacs --debug-init
 1940  bin/emacs-24.5 
 1941  bin/emacs-bare 
 1942  ll bin/emacs*
 1943  bin/emacs
 1944  cd .emacs.d
 1945  svn update
 1946  git pull
 1947  git fetch
 1948  ~/bin/emacs
 1949  XLIB_SKIP_ARGB_VISUALS=1 ~/bin/emacs
 1950  sed -n 's/.*\[\(.*\)]$/\1/p' backtrace |   addr2line -C -f -i -p -e bin/emacs
 1951  bin/emacs >backtrace 2>&1 ; sed -n 's/.*\[\(.*\)]$/\1/p' backtrace | addr2line -C -f -i -p -e bin/emacs
 1952  ~/bin/emacs >backtrace 2>&1 ; sed -n 's/.*\[\(.*\)]$/\1/p' backtrace | addr2line -C -f -i -p -e ~/bin/emacs
 1953  cat ~/.xsession-errors
 1954  cd
 1955  sudo apt-add-repository -y ppa:adrozdoff/emacs
 1956  sudo apt update
 1957  sudo apt install emacs25
 1958  cd ws/QPF/trunk/
 1959  ls
 1960  svn status
 1961  svn update
 1962  svn status
 1963  svn update --help
 1964  svn update --force
 1965  svn status
 1966  find . -name CMakeLists.txt 
 1967  find . -name CMakeLists.txt  -exec rm {} \+
 1968  svn status
 1969  svn update
 1970  svn status
 1971  svn revert CMakeLists.txt
 1972  svn status
 1973  svn status|grep D
 1974  svn status|grep D|awk '{print $3;}'
 1975  svn status|grep D|awk '{print $3;}' > FILES
 1976  svn revert $(cat FILES)
 1977  svn status
 1978  rm -rf .sonar/
 1979  rm FILES 
 1980  ls ~/Downloads/
 1981  ls -ltr ~/Downloads/
 1982  sudo apt-get install pmccabe 
 1983  pmccabe 
 1984  pmccabe --help
 1985  pmccabe -b
 1986  pmccabe -b qpf/deployer.cpp 
 1987  pmccabe -f -t -n qpf/deployer.cpp 
 1988  pmccabe -f -t qpf/deployer.cpp 
 1989  pmccabe -v -f -t qpf/deployer.cpp 
 1990  pmccabe -v -f -t -b qpf/deployer.cpp 
 1991  pmccabe -v -f -t -n qpf/deployer.cpp 
 1992  pmccabe -v -f -t $(find . \( -name "*.cpp" -o -name "*.h" -o -name "*.tcc" \) -print0 )
 1993  pmccabe -v -f -t $(find . \( -name "*.cpp" -o -name "*.h" -o -name "*.tcc" \) -print)
 1994  vi src/except.h
 1995  pmccabe -v -f -t $(find . \( -name "*.cpp" -o -name "*.h" -o -name "*.tcc" \) -print) > pmccabe.txt
 1996  vi +296 qpfhmi/mainwindow.h
 1997  cd
 1998  cd bin/
 1999  svn checkout svn://svn.code.sf.net/p/metrixplusplus/code/mainline Metrix++
 2000  ps x
 2001  kill -9 19297
 2002  cd 
 2003  cd ws/QPF/trunk/
 2004  python /home/jcgonzalez/bin/Metrix++/metrix++.py view
 2005  python /home/jcgonzalez/bin/Metrix++/metrix++.py help
 2006  python /home/jcgonzalez/bin/Metrix++/metrix++.py collect --help
 2007  python /home/jcgonzalez/bin/Metrix++/metrix++.py collect
 2008  python /home/jcgonzalez/bin/Metrix++/metrix++.py view
 2009  python /home/jcgonzalez/bin/Metrix++/metrix++.py collect --help
 2010  python /home/jcgonzalez/bin/Metrix++/metrix++.py collect --std.general.size --std.code.todo.comments --std.code.todo.strings--std.code.complexity.cyclomatic --std.code.complexity.maxindent --std.code.lines.code --std.code.lines.preprocessor --std.code.lines.comments --std.code.lines.total --std.code.length.total --std.code.magic.numbers --std.code.magic.numbers.simplier --std.code.cpp.files='*.c,*.h,*.cpp,*.hpp,*.cc,*.hh,*.cxx,*.hxx,*.tcc'
 2011  python /home/jcgonzalez/bin/Metrix++/metrix++.py collect --std.general.size --std.code.todo.comments --std.code.todo.strings --std.code.complexity.cyclomatic --std.code.complexity.maxindent --std.code.lines.code --std.code.lines.preprocessor --std.code.lines.comments --std.code.lines.total --std.code.length.total --std.code.magic.numbers --std.code.magic.numbers.simplier --std.code.cpp.files='*.c,*.h,*.cpp,*.hpp,*.cc,*.hh,*.cxx,*.hxx,*.tcc'
 2012  python /home/jcgonzalez/bin/Metrix++/metrix++.py view
 2013  python /home/jcgonzalez/bin/Metrix++/metrix++.py help
 2014  python /home/jcgonzalez/bin/Metrix++/metrix++.py view
 2015  python /home/jcgonzalez/bin/Metrix++/metrix++.py view --help
 2016  python /home/jcgonzalez/bin/Metrix++/metrix++.py export --help
 2017  python /home/jcgonzalez/bin/Metrix++/metrix++.py export 
 2018  python /home/jcgonzalez/bin/Metrix++/metrix++.py export --std.code.complexity.maxindent
 2019  python /home/jcgonzalez/bin/Metrix++/metrix++.py view --std.code.complexity.maxindent
 2020  python /home/jcgonzalez/bin/Metrix++/metrix++.py view std.code.complexity.maxindent
 2021  python /home/jcgonzalez/bin/Metrix++/metrix++.py collect --std.code.complexity.maxindent
 2022  python /home/jcgonzalez/bin/Metrix++/metrix++.py view
 2023  python /home/jcgonzalez/bin/Metrix++/metrix++.py export
 2024  python /home/jcgonzalez/bin/Metrix++/metrix++.py export|less
 2025  python /home/jcgonzalez/bin/Metrix++/metrix++.py collect --std.general.size --std.code.todo.comments --std.code.todo.strings --std.code.complexity.cyclomatic --std.code.complexity.maxindent --std.code.lines.code --std.code.lines.preprocessor --std.code.lines.comments --std.code.lines.total --std.code.length.total --std.code.magic.numbers --std.code.magic.numbers.simplier --std.code.cpp.files='*.c,*.h,*.cpp,*.hpp,*.cc,*.hh,*.cxx,*.hxx,*.tcc'
 2026  python /home/jcgonzalez/bin/Metrix++/metrix++.py export > QPF.metrix++
 2027  awk '{type=$2;print type;}' QPF.metrix++ 
 2028  awk -F, '{type=$3;print type;}' QPF.metrix++ 
 2029  awk -F, '{type=$3;f=file"."type;print  >> file;}' QPF.metrix++ 
 2030  awk -F, 'BEGIN{file=FILENAME;}{type=$3;f=file"."type;print  >> f;}' QPF.metrix++ 
 2031  ll
 2032  rm .file .function .class .global  .type .struct .namespace 
 2033  ll
 2034  man awk\
 2035  awk -F, '{type=$3;f=FILENAME"."type;print  >> f;}' QPF.metrix++ 
 2036  ll
 2037  less QPF.metrix++.class 
 2038  head -1 QPF.metrix++
 2039  for i in QPF.metrix++.*; do mv $i k; (head -1 QPF.metrix++ ; cat k)  > $i ; done
 2040  ll
 2041  less QPF.metrix++.class 
 2042* 
 2043  less QPF.metrix++.file
 2044  sv info
 2045  svn info
 2046  ftp ftp.jcgonzalez.org
 2047  gedit infix/infixeval.tcc 
 2048  gedit libcomm/r2rpeer.cpp 
 2049  gedit src/config.cpp 
 2050  emacs --debug-init &
 2051  cd
 2052  emacs --debug-init
 2053  ll bin/emacs
 2054  ll bin/emacs*
 2055  locate emacs-25.1
 2056  cd /home/jcgonzalez/local/src/emacs/src/emacs-25.1.50.1
 2057  cd /home/jcgonzalez/local/src/emacs/src/
 2058  ls
 2059  svn info
 2060  cvs info
 2061  cvs status
 2062  git info
 2063  git status
 2064  git fetch
 2065  ll
 2066  ls -s
 2067  ./emacs
 2068  ./temacs 
 2069  make
 2070  make clean
 2071  ll
 2072  cd ..
 2073  ll
 2074  git fetch
 2075  ./configure
 2076  locate Clear
 2077  cat /home/jcgonzalez/ws/personal/QPF.git/misc/ClearForQPF.sh
 2078  scp -R /home/jcgonzalez/shared/qla-testimages eucops@euclodeen1-2:Work/jcg/
 2079  scp -r /home/jcgonzalez/shared/qla-testimages eucops@euclodeen1-2:Work/jcg/
 2080  cd
 2081  cd ws/QDT
 2082  cd euclidqla/
 2083  cd trunk/
 2084  svn info
 2085  svn update
 2086  svn copy https://euclid.esac.esa.int/svn/ESA/SOC/SOC-3-DEV/SOC-3-07-QLook/QDT/euclidqla/trunk https://euclid.esac.esa.int/svn/ESA/SOC/SOC-3-DEV/SOC-3-07-QLook/QDT/euclidqla/tags/PREMERGE-consolidated_report_to_trunk
 2087  cd ../branches/consolidated_report/
 2088  svn status
 2089  cd -
 2090  svn merge https://euclid.esac.esa.int/svn/ESA/SOC/SOC-3-DEV/SOC-3-07-QLook/QDT/euclidqla/branches/consolidated_report
 2091  less euclidqla/functions/diagnostics/cosmic_ray_diagnostic.py
 2092  find . -name "*.py" -print | xargs grep "#JCG" 
 2093  svn commit -m "Merge of consolidated_report done" #find . -name "*.py" -print | xargs perl -p -i -e "#JCG" 
 2094  find . -name "*.py" -print | xargs perl -p -i -e "s/[ \t]+#JCG.*\n//g;" 
 2095  find . -name "*.py" -print | xargs grep "#JCG" 
 2096  svn diff
 2097  svn revert
 2098  svn help revert 
 2099  svn revert .
 2100  svn revert -R .
 2101  find . -name "*.py" -print | xargs grep "#JCG" 
 2102  man perlre
 2103  man perl
 2104  man perlre
 2105  man perlreg
 2106  find . -name "*.py" -print | xargs perl -p -i -e "s/^[ \t]+#JCG.*?\n//gs;" 
 2107  svn diff
 2108  svn diff|egrep "^-"
 2109  svn commit -m "Old #JCG comment lines (from consolidated_report branch) removed"
 2110  cd ../../../QPF/
 2111  ll
 2112  cd trunk/
 2113  ll
 2114  svn update
 2115  ls -ltr
 2116  mkdir build
 2117  cd build/
 2118  ccmake ..
 2119  make
 2120  cd ..
 2121  rm -rf build/
 2122  cd scripts/
 2123  ./BuildQPF.sh -c -i -b -r -p
 2124  cd /home/jcgonzalez/opt/include
 2125  ll
 2126  ln -s ../zmq/include/zmq.hpp .
 2127  ln -s ../pcre2/include/PCRegEx.h .
 2128  ./BuildQPF.sh -c -i -b -r -p
 2129  cd -
 2130  ./BuildQPF.sh -c -i -b -r -p
 2131  cd -
 2132  for i in /opt/cots/include/* ; do ln -sf $i . ; done
 2133  mv ../lib/ ; for i in /opt/cots/lib/lib* ; do ln -sf $i . ; done
 2134  ll
 2135  rm lib*
 2136  cd ../lib/ ; for i in /opt/cots/lib/lib* ; do ln -sf $i . ; done
 2137  ll
 2138  cd
 2139  cd ws/QPF/trunk/scripts/
 2140  ./BuildQPF.sh -c -i -b -r -p
 2141  ls -l
 2142  cd ..
 2143  svn status
 2144  svn commit -m "Method transmissionsHandler refactored" libcomm/r2rpeer.*
 2145  ll
 2146  history > h
