from openpyxl import load_workbook
import os
import csv
from datetime import datetime
import re
from enum import Enum
import shutil
import subprocess
from time import sleep


template_path: str = "schedule_template.xlsx"
timeStamp = datetime.today().strftime('%Y-%m-%d_%H%M%S')
resultPath = "result_" + timeStamp + ".xlsx"
csvSourcePath: str = "PRDAT/result.csv"
resultDirPath = "RESULT"
#check and create result dir
try:
    os.mkdir(resultDirPath)
except FileExistsError:
    pass
resultPath = os.path.join(resultDirPath, resultPath)
print(resultPath)

shutil.copyfile(template_path, resultPath)