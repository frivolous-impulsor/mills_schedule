#all time related matrix is of 30x7, indicating 30 time slots for 7 days a week. Each time slot represent 30min. In total 15hrs are represented each day, from 8am to 11pm
#the goal of scheduling is that for each student s 1)s is assigned as close to their desired hour per week and 2)hrs being assigned to s is composed of their favored hour as major as possible
#time slices are to be grouped to chuncks like 2hrs -> 3.5hrs. Scheduling alogrithm being input a time chunck, and allocate 1 to 3 students such chunk based on their toBeAssigned hours and if those hours are their favored time

class Student:
    def __init__(self, desiredHour) -> None:
        self.macId: str
        self.rookie: bool
        self.desiredHour: int = desiredHour #generally 10hrs, subject to individual case
        self.toBeAssigned: int = desiredHour #originally each student hasn't been assigned any hours, thus their hours to be assigned is thir desiredHour
        self.availableTimeGrid: list[list[int]] #a 30x7 matrix indicating preference, empty:=no shift   1:=can have shift   2:=love to have shift
    
    def fetchInfo(self, excelAddress) -> int: #get macId, rookieness, availableTimeGrid from excel time sheet and store in self.macId
        "todo"


class StudentsGroup:
    def __init__(self) -> None:
        self.studentList: dict  #macId: student object
        self.studentHourList: dict #macId: hours to be assigned

    def addStudent(self, student: Student):
        if student.macId not in self.studentList.keys():
            self.studentList[student.macId] = student
            self.studentHourList[student.macId] = student.toBeAssigned
            return 0
        else:
            print(f"student with macId {student.macId} already exists. check excel files for duplicate submitt")
            return 1
        
class indexedPQ:
    

class schedule:
    def __init__(self) -> None:
        self.students: StudentsGroup
        self.numberStaffOnHourChart: list[list[int]] # a 30x7 matrix indicating number of students needed for each hour
        self.finalSchedule: list[list[str]] # a 30x7 matrix indicating students in each shift


    def schedule(self):
        