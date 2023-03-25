echo "THIS IS ONLY WORKING FOR SHORT JOBS "
source ~/SetupAPP.sh
export DISPLAY=localhost:0.0
root -l TRExFitter_task.C 
