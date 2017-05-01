import sys
import os
import os.path
path="/Users/vasanthreddy/Desktop/projectrep"
flst=os.listdir(path)
for fil in flst:
	if os.path.isfile(path+'/'+fil)and (fil.startswith("mlfq") or fil.startswith("ori") or fil.startswith("prio") or fil.startswith("log")):
		print("Running file :"+fil)
		comm1=fil+' |grep "Wa" |cut -f4 -d" " >repw_'+fil
		comm2=fil+' |grep "Tu" |cut -f4 -d" " >rept_'+fil
        #print(comm1)
        #print(comm2)
		os.system('cat '+comm1)
		lines = [float(line.rstrip('\n')) for line in open('repw_'+fil)]
		print("Avg wait for "+fil+" ="+str(float(sum(lines)/10)))
		os.system('cat '+comm2)
		lines = [float(line.rstrip('\n')) for line in open('rept_'+fil)]
		print("Avg turn for "+fil+" ="+str(float(sum(lines)/10))) 

