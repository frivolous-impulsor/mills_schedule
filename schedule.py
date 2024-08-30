

class Student:
    def __init__(self, desiredHour) -> None:
        self.macId: str
        self.desiredHour: int = desiredHour #generally 10hrs, subject to individual case
        self.availableTimeGrid: list[list[int]] #a 15x7 matrix indicating preference, 0:=no shift   1:=can have shift   2:=love to have shift
    
    def fetchMacId(self, excelAddress) -> str: #get macId from excel time sheet and store in self.macId
        "todo"

    def excel2timeGrid(self, excelAddress) -> None: #get AvailableTimeGrid as 2x2 int matrix from excel time sheet and store in object attriute 
        "todo"

class StudentsGroup:
    def __init__(self) -> None:
        self.studentList: dict

    def addStudent(self, student: Student):
        if student.macId not in self.studentList.keys():
            self.studentList[student.macId] = student
            return 0
        else:
            print(f"student with macId {student.macId} already exists. check excel files for duplicate submitt")
            return 1

class schedule:
    def __init__(self) -> None:
        self.students: StudentsGroup
        self.finalSchedule: list[list[str]] # a 
