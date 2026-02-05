```bash
docker compose exec backend bash -c "cd /project/boinc_project && ./bin/feeder"
```

```bash
docker compose exec backend bash -c "cd /project/boinc_project && ./bin/transitioner -once"
```

```bash
docker compose exec backend bash -c "cd /project/boinc_project && ./bin/validator"
docker compose exec backend bash -c "cd /project/boinc_project && ./bin/assimilator"
```
