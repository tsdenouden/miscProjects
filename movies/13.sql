-- Gets the names of the people who worked with Kevin Bacon in a movie 
SELECT DISTINCT name FROM people WHERE id in (SELECT person_id FROM stars WHERE movie_id in (SELECT id FROM movies WHERE id in (SELECT movie_id FROM stars WHERE person_id in (SELECT id FROM people WHERE name = "Kevin Bacon" AND birth = 1958)))) AND people.name <> "Kevin Bacon";
