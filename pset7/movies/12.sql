SELECT title FROM movies
WHERE movies.id IN
    (SELECT movies.id FROM people
     JOIN stars ON people.id = person_id JOIN
     movies ON stars.movie_id = movies.id
     WHERE name = "Johnny Depp")
and movies.id IN
    (SELECT movies.id FROM people
     JOIN stars ON people.id = person_id JOIN
     movies ON stars.movie_id = movies.id
     WHERE name = "Helena Bonham Carter");