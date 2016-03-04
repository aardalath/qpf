export PATH=.:$HOME/bin:$HOME/local/bin:/opt/cots/bin:/opt/Qt/5.4/gcc_64/bin:/opt/Qt/Tools/QtCreator/bin:$PATH
export LD_LIBRARY_PATH=.:$HOME/local/lib:/opt/cots/lib:/opt/Qt/5.4/gcc_64/lib:$LD_LIBRARY_PATH
export PKG_CONFIG_PATH=/opt/cots/lib/pkgconfig

# Add paths for QPF tests
ldpths="."
ldpth="$ldpth:$HOME/ws/personal/QPF/src"
ldpth="$ldpth:$HOME/ws/personal/QPF/json"
ldpth="$ldpth:$HOME/ws/personal/libcomm/src"
ldpth="$ldpth:$HOME/ws/personal/QPF/build/debug/src"
ldpth="$ldpth:$HOME/ws/personal/QPF/build/debug/json"
ldpth="$ldpth:$HOME/ws/personal/libcomm/build/debug/src"

export LD_LIBRARY_PATH=$ldpth:$LD_LIBRARY_PATH

export QPFHMI=/home/jcgonzalez/ws/QPF/build/debug/qpfhmi/qpfhmi

# Set environment to run PostgreSQL
source /opt/PostgreSQL/9.4/pg_env.sh

# Mount shared folder
sudo mount.vboxsf shared /media/jcgonzalez/shared/

# Restart docker service
sudo service docker restart

# Set /var/run/qpf folder
RUNPATH=/var/run/qpf
sudo mkdir -p ${RUNPATH}
sudo chmod 777 ${RUNPATH}
sudo chown jcgonzalez.jcgonzalez ${RUNPATH}

# Launch emacs
emacs &

# Launch pgAdmin
/opt/PostgreSQL/9.4/pgAdmin3/bin/pgadmin3 &

# Set /var/run/qpf folder
RUNPATH=/var/run/qpf
sudo mkdir -p ${RUNPATH}
sudo chmod 777 ${RUNPATH}
sudo chown jcgonzalez.jcgonzalez ${RUNPATH}
