from openpyxl import load_workbook
import os
import csv

MAX_SLOT_IN_DAY: int = 10
MAX_DAY_IN_WEEK: int = 7

def getMasterFilePath():
    files: list[str] = [file for file in os.listdir('.') if (os.path.isfile(file) and "schedule" in file and "template" in file and file[-5:] == ".xlsx")]
    if len(files) != 1:
        raise FileNotFoundError("Ensure exactly one \"schedule_template.xlsx\" at root dir")
    return files[0]

def analyzeMaster(masterPath: str):
    hourMat = [[0 for _ in range(MAX_SLOT_IN_DAY)] for _ in range(MAX_DAY_IN_WEEK)]
    peopleNeededMat = [[0 for _ in range(MAX_SLOT_IN_DAY)] for _ in range(MAX_DAY_IN_WEEK)]
    workbook = load_workbook(masterPath)
    sheet = workbook.active
    for day in range(1, MAX_DAY_IN_WEEK*2 +1):
        for slot in range(1, MAX_SLOT_IN_DAY*2+1):
            val = sheet.cell(slot, day).value
            print(val)


def main():
    masterPath = getMasterFilePath()
    analyzeMaster(masterPath)



main()