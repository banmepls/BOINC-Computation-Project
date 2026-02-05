```bash
docker compose exec backend bash -c "cd /project/boinc_project && ./bin/feeder"
```

```bash
docker compose exec backend bash -c "cd /project/boinc_project && ./bin/transitioner --one_pass"
```

```bash
docker compose exec backend bash -c "cd /project/boinc_project && ./bin/sample_trivial_validator --app pi_compute --one_pass"
docker compose exec backend bash -c "cd /project/boinc_project && ./bin/sample_trivial_validator --app euler --one_pass"
docker compose exec backend bash -c "cd /project/boinc_project && ./bin/catalan_validate --app catalan --one_pass"
```

```bash
docker compose exec backend bash -c "cd /project/boinc_project && ./bin/sample_assimilator --app pi_compute --one_pass"
docker compose exec backend bash -c "cd /project/boinc_project && ./bin/sample_assimilator --app euler --one_pass"
docker compose exec backend bash -c "cd /project/boinc_project && find upload -name '*catalan*' -type f | head -1 | xargs -I {} ./bin/catalan_assimilate {} 2>/dev/null || echo 'Nu am găsit fișiere catalan pentru assimilare'"
```
