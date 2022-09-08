--Create a new table for history of stocks bought
CREATE TABLE history (
    transaction_id int,
    symbol TEXT NOT NULL,
    shares int NOT NULL,
    price int NOT NULL,
    person_id int,
    PRIMARY KEY(transaction_id),
    FOREIGN KEY(person_id) REFERENCES users(id)
);