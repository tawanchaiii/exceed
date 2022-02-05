from typing import Optional
from fastapi import FastAPI
from pydantic import BaseModel
import random
class Item(BaseModel):
    name: str
    surname: str

app = FastAPI()

@app.get("/")
def read_root():
    return {"Hello": "World"}

@app.post("/name")
def create_item(item: Item):
    return {
        "name": item.name,
        "surname": item.surname,
        "result": (item.name + " " + item.surname.upper())
    }
@app.get("/items/{item_id}")
def read_item(item_id: int,q: Optional[str] = None):
    return {"item_id": item_id, "q": q}

@app.put("/items/{item_id}")
def update_item(item_id: int,item: Item):
    return {"item_name": item.name, "item_id": item_id}
