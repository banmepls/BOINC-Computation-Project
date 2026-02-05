#!/bin/bash
echo "=== BOINC PROJECT CHECK FROM HOST ==="
echo ""
echo "1. Checking Docker containers:"
docker ps --format "table {{.Names}}\t{{.Status}}\t{{.Ports}}" | grep -E "(apache|backend|db)"
echo ""
echo "2. Checking Apache web interface:"
echo "   Testing: http://localhost/boinc_project"
HTTP_STATUS=$(curl -s -o /dev/null -w "%{http_code}" http://localhost/boinc_project 2>/dev/null || echo "FAILED")
if [[ "$HTTP_STATUS" =~ ^(200|301|302)$ ]]; then
    echo "   ✅ Apache is responding (HTTP $HTTP_STATUS)"
else
    echo "   ❌ Apache not accessible (Status: $HTTP_STATUS)"
    echo "   Try: docker compose restart apache"
fi
echo ""
echo "3. Checking backend container:"
if docker compose exec backend echo "Backend container is running" 2>/dev/null; then
    echo "   ✅ Backend container is accessible"
    
    # Check database from backend
    echo "   Checking database..."
    docker compose exec backend mysql -u boincadm -pboinc123 -h db boinc_project -e "SELECT COUNT(*) as 'Work Units' FROM workunit;" 2>/dev/null || echo "   Could not access database"
else
    echo "   ❌ Backend container not accessible"
fi
echo ""
echo "4. Quick test instructions:"
echo "   a) Open browser: http://localhost/boinc_project"
echo "   b) Check database: docker compose exec backend bash -c 'mysql -u boincadm -pboinc123 -h db boinc_project -e \"SELECT * FROM workunit;\"'"
echo "   c) Run demo: docker compose exec backend bash -c 'cd /project/boinc_tutorial/src && ./demo_complete.sh'"
