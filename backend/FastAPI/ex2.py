from typing import Optional

from fastapi import FastAPI
from pydantic import BaseModel

app =  FastAPI()

student = {}

class Student(BaseModel):
    id: str
    name: str
    age: Optional[int]

@app.post("/new")
def new_student[student]