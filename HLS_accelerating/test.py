from functools import reduce
import os
import subprocess as sp
import xml.etree.ElementTree as ET

# Please modify your Vitis HLS installation path here
vitis_dir = "D:/pku/dasanxia/Xilinx/Vitis_HLS/2020.2/bin/vitis_hls.bat"
# Please fill the command to run your Python here.
python_cmd = "python.exe"

src_dir = "srcs"

# You may set where to store the logging information, 
# and the commented line dismisses the information.
log_file = "test.log"
# log_file = os.devnull

#----- DO NOT modify the code below -----
cur_dir = os.getcwd()

def checkCSim():
  print("Check C-Simulation Result: ", end='', flush=True)
  csim_report_dir = os.path.join(cur_dir, "hw1_mm", "solution1", "csim", "report", "mm_csim.log")
  try:
    with open(csim_report_dir, "r") as report:
      result = report.readlines()[-2].rstrip()
      if result=="INFO: [SIM 1] CSim done with 0 errors.":
        return True
  except BaseException:
    return False

score_rules = [
  [11999999, 95, 90, 40, 60, 10],
  [1699999, 95, 90, 40, 60, 20],
  [1699999, 30, 90, 25, 50, 30],
  [699999, 30, 15, 10, 15, 50],
  [99999, 20, 10, 5, 10, 80],
  [79999, 20, 10, 5, 10, 90],
  [19999, 20, 10, 5, 10, 100]
]

def scoreSynth():
  print("Check Synthesis Result: ")
  synth_report_dir = os.path.join(cur_dir, "hw1_mm", "solution1", "syn", "report", "mm_csynth.xml")
  try:
    tree = ET.parse(synth_report_dir)
  except FileNotFoundError:
    print('Can\'t found the report file (xml). Please check the logging information.' )
    exit(0)
  root = tree.getroot()
  result = []
  result.append(int(root.findtext('./PerformanceEstimates/SummaryOfOverallLatency/Best-caseLatency')))
  print("| Latency = ", result[0], end=' | ', flush=True)

  res_path = './AreaEstimates/Resources/'
  available_res_path = './AreaEstimates/AvailableResources/'
  items = ['BRAM_18K', 'DSP', 'FF', 'LUT']
  for item in items:
    res = int(root.findtext(res_path + item)) 
    available_res = int(root.findtext(available_res_path + item))
    result.append(res * 100 // available_res)
    print(item + ' = ' + str(result[-1]), end=' | ', flush=True)
  
  print('')
  le = lambda rule : reduce(lambda a, b: a and b, [a<=b for a, b in zip(result, rule)], True)
  score = max([rule[-1] if le(rule) else 0 for rule in score_rules])
  print("Score is " + str(score) + '. Congratulations!')
  

if __name__=="__main__":
  with open(log_file, 'w') as flog:
    print(f"Generate mat.txt to {src_dir} : ", end='', flush=True)
    sp.run([python_cmd, "tb_gen.py", src_dir], stdout=flog, stderr=flog)
    print("Finished")
    
    print("Create Vitis HLS Project, Add Files, Run C-Simulation and Synthesis : ", end='', flush=True)

    try:
      sp.run([vitis_dir, "script.tcl"], stdout=flog, stderr=flog, timeout=60)
    except sp.TimeoutExpired:
      print("Timeout!", "Score is 0. Come on!", sep='\n')
      exit(0)
    except sp.SubprocessError:
      print("Failed!", "Score is 0. Come on!", sep='\n')
      exit(0)
    else:
      print("Finished")
    
    if not checkCSim():
      print("Failed!", "Score is 0. Please check C-Simulation.", sep='\n')
      exit(0)
    else:
      print("Passed!")
      scoreSynth()