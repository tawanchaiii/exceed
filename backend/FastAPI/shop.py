from fastapi import FastAPI
from typing import Optional
from pydantic import BaseModel 
# input the body in specific form (class) to use in our function #


app = FastAPI()

class Item(BaseModel):
	name : str
	price : float
	discount: Optional[float] = 0


@app.get("/")
def root():
	return {"Hello":"EXCEED"}
'''
#have put delete post get  

@app.post("/")
def root_post():
	return {"Hello":"exceed from post"}

@app.put("/")
def root_put():
	return {"Hello":"exceed from put"}

@app.delete("/")
def root_delete():
	return {"Hello":"exceed from delete"}
'''
@app.get("/items/{item_id}") #force parameter to be subURL 
def get_item(item_id: int, q: Optional[str] = None):
	return {"items_id":item_id , "q from get": q}


@app.get("/items/") #if need the optional parameter delete variable in sub
def get_item1( q: Optional[str] = None):
	return {"Hello":"Please Insert parameter." , "q from get": q}


@app.post("/items/{item_id}") #force to pass the body as parameter in bodybase format
def post_item(item_id: int, item: Item):
	return { "items_id":item_id , 
			"name": item.name,
			"price":item.price,
			"now have discount":item.discount }